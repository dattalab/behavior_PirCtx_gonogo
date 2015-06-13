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
// ISI duration (ms); -1 for exponential distribution
// Start of assessment window (int, ms) for licking
// Duration of assessment window
void odor_stimulation(int mode, int current_block, int nb_trials, int block_order[], int duration_odor_sampling, int duration_interstimulus_interval, int nb_odors, int odors[], String odor_name[], int default_flows[], int carrier_flows[], int odor_flows[]) {
  // Setup the olfactometer
  idleOlfacto(); // put the olfacto in iddle mode

  // Randomize ITI if ITI = 0
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
  writeOut(fstringF(F("%i,BIP,%i,%i,%i"), millis(), current_block, nb_trials, nb_odors));

  // # Block initiation duration
  // time,BID,block_id,odor_sampling,wait_time,outcome,ISI
  writeOut(fstringF(F("%i,BID,%i,%i"), millis(), current_block, duration_odor_sampling, duration_interstimulus_interval));

  // # Block initiation odor list
  // For each odor:
  // time,BIO,block_id,odor_id,odor_name,odor_valence,valve
  for (int i = 0; i < nb_odors; i++) {
    char name_odor[30];
    odor_name[i].toCharArray(name_odor, 30);
    writeOut(fstringF(F("%i,BIO,%i,%i,%s,%i"), millis(), current_block, i + 1, name_odor, odors[i]));
  }

  sendTriggerTTL();

  // ### Trial initiation ###
  for (int trial_id = 1; trial_id < (nb_trials + 1); trial_id++) {
    checkSerial();
    // Check air flows
    if (lastRequestedFlows[0] != carrier_flows[block_order[trial_id - 1]]) {
      for (int i = 0; i < carriers_nb; i++) {
        updateFlowOlfacto(0, carriers[i], carrier_flows[block_order[trial_id - 1]]);
      }
    }
    if (lastRequestedFlows[1] != odor_flows[block_order[trial_id - 1]]) {
      for (int i = 0; i < bankflows_nb; i++) {
        updateFlowOlfacto(1, bankflows[i], odor_flows[block_order[trial_id - 1]]);
      }
    }
    writeOut(fstringF(F("//%i,ITI,%i,%i,%i"), millis(), current_block, trial_id, duration_ITI[trial_id - 1]));
    delay(duration_ITI[trial_id - 1]);

    // Send a command to log trial initiation
    // time,O,block_id,trial_id,valve_identity
    writeOut(fstringF(F("%i,O,%i,%i,%i"), millis(), current_block, trial_id, block_order[trial_id - 1] + 1));

    // we need to initialize a few variables
    unsigned long start_count_time = millis();

    // start odor delivery from olfactometer
    digitalWrite(TTL, HIGH);
    openValve(odors[block_order[trial_id - 1]]);
    delay(duration_odor_sampling);
    closeValve(odors[block_order[trial_id - 1]]);
    digitalWrite(TTL, LOW);

    // Running status management
    checkPauseResume();
    if (*pRunningState == 2) {
      trial_id = nb_trials + 1;
    }
  }
}
