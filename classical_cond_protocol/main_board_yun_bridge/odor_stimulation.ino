// This function is used to make a training block
// Mode: 1/2 (classical conditioning / go/no-go)
// Nb of trials (int)
// Order of the odors (array int, length has to match nb of trials): id of the odors
// Duration of odor sampling (int, ms)
// Nb of odors (int)
// Odor valves (array int, length has to match nb of odors): valves on the olfactometer board
// Odor valence (array int, length has to match nb of odors): outcome codes of the different odors (1: water reward, 0: nothing)
// Odor names (array string, length has to match nb of odors)
//
// MODE 1: classical conditioning, the US is delivered independently of the mouse licking behavior
// Parameters:
// Waiting duration (int, ms)
// Duration of outcome phase (int, ms)
// ISI duration (int, ms): -1 for exponential distribution
// Start of assessment window (int, ms)
// Duration of assessment window (int, ms)
//
// MODE 2: go/no-go task, water reward is delivered only if the mouse has licked within the assessment window
// Parameters:
// Waiting duration (int, ms): trial time after odor delivery
// Delay between licking and water reward delivery (int, ms)
// ISI duration (ms); 0 for exponential distribution
// Start of assessment window (int, ms) for licking
// Duration of assessment window

void odor_stimulation(int mode, int current_block, int nb_trials, int block_order[], int duration_odor_sampling, int duration_wait, int duration_outcome, int duration_interstimulus_interval, int start_assessment_window, int duration_assessment_window, int step_punishment_isi, int nb_odors, int odors[], int odor_valence[], String odor_name[], int default_flows[], int carrier_flows[], int odor_flows[]) {
  // Setup the olfactometer
  setDurationOlfacto(duration_odor_sampling); // send duration to olfactometer
  idleOlfacto(); // put the olfacto in iddle mode

  updateFlowOlfacto(0, default_flows[0]);
  updateFlowOlfacto(1, default_flows[1]);

  // Randomize ITI if ITI = -1
  int duration_ITI[nb_trials];
  if (duration_interstimulus_interval == 0) {
    randITI(duration_ITI, nb_trials, 3.0, 0.0, 25);
    randomizeArray(duration_ITI, nb_trials);
  }
  else {
    for (int t = 0; t < nb_trials; t++) {
      duration_ITI[t] = duration_interstimulus_interval;
    }
  }

  checkSerial();

  // ### Send parameters ###
  // # Block initiation parameters
  // time,BIP,block_ID,nb_trials,nb_odors
  //writeOut((String) millis() + F(",BIP,") + String(current_block) + F(",") + String(nb_trials) + F(",") + String(nb_odors));
  writeOut(fstringF(F("%lu,BIP,%i,%i,%i"), millis(), current_block, nb_trials, nb_odors));

  // # Block initiation duration
  // time,BID,block_id,odor_sampling,wait_time,outcome,ISI
  writeOut(fstringF(F("%lu,BID,%i,%i,%i,%i,%i"), millis(), current_block, duration_odor_sampling, duration_wait, duration_outcome, duration_interstimulus_interval));

  // # Block initiation assessment window
  // time,BIW,start_time,duration
  writeOut(fstringF(F("%lu,BIW,%i,%i,%i"), millis(), current_block, start_assessment_window, duration_assessment_window));

  // # Block initiation odor list
  // For each odor:
  // time,BIO,block_id,odor_id,odor_name,odor_valence,valve
  for (int i = 0; i < nb_odors; i++) {
    writeOut((String) millis() + F(",BIO,") + current_block + F(",") + (i + 1) + F(",") + odor_name[i] + F(",") + odor_valence[i] + F(",") + odors[i]);
  }

  // ### Trial initiation ###
  for (int trial_id = 1; trial_id < (nb_trials + 1); trial_id++) {
    checkSerial();
    checkFeedbackOlfacto();
    // Check air flows
    if (lastRequestedFlows[0] != carrier_flows[block_order[trial_id - 1]]) {
      updateFlowOlfacto(0, carrier_flows[block_order[trial_id - 1]]);
    }
    if (lastRequestedFlows[1] != odor_flows[block_order[trial_id - 1]]) {
      updateFlowOlfacto(1, odor_flows[block_order[trial_id - 1]]);
    }
    writeOut((String) millis() + ",ITI," + current_block + "," + trial_id + "," + duration_ITI[trial_id - 1]);
    delay(duration_ITI[trial_id - 1]);

    // Send a command to log trial initiation
    // time,O,block_id,trial_id,valve_identity
    writeOut(fstringF(F("%lu,O,%i,%i,%i"), millis(), current_block, trial_id, block_order[trial_id - 1] + 1));

    // we need to initialize a few variables
    byte odor_on = 1;
    byte outcome_on = 0;
    int outcome_code = odor_valence[block_order[trial_id - 1]];
    unsigned long start_count_time = millis();
    byte lastLickState = 0;
    byte lickState = 0;
    int countLicks = 0;
    byte statusFlow = 0;

    // state monitors the status of the trial
    // 0: odor delivery started, 1: odor deliveryy ended, 2: outcome delivery started, 3: outcome delivery ended, 4: end, initiate ITI
    int state = 0;

    // start odor delivery from olfactometer
    stimulusOlfacto(odors[block_order[trial_id - 1]]);

    if (mode == 1) { // Option 1, phase 2
      while (state < 4) {
        // # Check the status of the lickometer
        lickState = digitalRead(lickInPin); // read status
        if (lickState != lastLickState) { // if status has changed
          if (lickState == HIGH) { // if a lick just started
            countLicks++;
            // Log the lick
            // time,L,block_id,trial_id,lick_id,1
            writeOut(fstringF(F("%lu,L,%i,%i,%i,1"), millis(), current_block, trial_id, countLicks));
          }
          else { // if a lick just ended
            // Log the lick
            // time,L,block_id,trial_id,lick_id,0
            writeOut(fstringF(F("%lu,L,%i,%i,%i,0"), millis(), current_block, trial_id, countLicks));
          }
          lastLickState = lickState;
        }
        switch (state)
        {
          case 0:
            if (millis() > (start_count_time + duration_odor_sampling)) {
              state = 1;
              checkFeedbackOlfacto();
            }
            break;
          case 1:
            if (millis() > (start_count_time + duration_odor_sampling + duration_wait)) {
              state = 2;
              checkFeedbackOlfacto();
            }
            break;
          case 2:
            if ((outcome_on == 0) && (outcome_code != 0)) {
              switch (outcome_code) {
                case 1:
                  deliverWaterReward(reward_solenoid_length);
                  break;
                case 2:
                  digitalWrite(punishmentOutPin, HIGH);
                  break;
              }
              outcome_on = outcome_code;
              // log outcome delivery
              // time,US,block_id,trial_id,outcome_code,1
              writeOut(fstringF(F("%lu,US,%i,%i,%i,1"), millis(), current_block, trial_id, outcome_code));
            }
            if (millis() > (start_count_time + duration_odor_sampling + duration_wait + duration_outcome)) {
              state = 3;
            }
            break;
          case 3:
            if (outcome_code == 2) {
              digitalWrite(punishmentOutPin, LOW);
            }
            state = 4;
            break;
          default:
            state = 0;
            break;
        }
      }
    }

    else if (mode == 2) { // go/no-go task
      unsigned long trigger_time = 0;
      while (state < 4) {
        // # Check the status of the lickometer
        lickState = digitalRead(lickInPin); // read status
        if (lickState != lastLickState) { // if status has changed
          if (lickState == HIGH) { // if a lick just started
            countLicks++;
            unsigned long lick_time = millis();
            // Log the lick
            // time,L,block_id,trial_id,lick_id,1
            writeOut(fstringF(F("%lu,L,%i,%i,%i,1"), millis(), current_block, trial_id, countLicks));

            if ((outcome_on == 0) && (outcome_code == 1) && (lick_time > start_count_time + start_assessment_window) && (lick_time < (start_count_time + start_assessment_window + duration_assessment_window))) {
              outcome_on = 3;
              trigger_time = lick_time;
            }
            else if((outcome_code == 0) && (outcome_on == 0)){
              outcome_on=4;
              if(trial_id < nb_trials){
                duration_ITI[trial_id]=duration_ITI[trial_id]+step_punishment_isi;
              }
            }
          }
          else { // if a lick just ended
            // Log the lick
            // time,L,block_id,trial_id,lick_id,0
            writeOut(fstringF(F("%lu,L,%i,%i,%i,0"), millis(), current_block, trial_id, countLicks));
          }
          lastLickState = lickState;
        }
        if ((outcome_on == 3) && (millis() > (trigger_time + duration_outcome))) {
          deliverWaterReward(reward_solenoid_length);
          outcome_on = 2;
          // log outcome delivery
          // time,US,block_id,trial_id,outcome_code,1
          writeOut(fstringF(F("%lu,US,%i,%i,%i,1"), millis(), current_block, trial_id, outcome_code));
        }
        if (millis() > (start_count_time + duration_odor_sampling + duration_wait)) {
          state = 4;
        }
      }
    }

    // Running status management
    checkFeedbackOlfacto();
    checkPauseResume();
    if (*pRunningState == 2) {
      trial_id = nb_trials + 1;
    }
  }
}
