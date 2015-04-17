void odor_stimulation(int mode, int current_block, int nb_trials, int block_order[], int duration_odor_sampling, int duration_wait, int duration_outcome, int duration_interstimulus_interval, int start_assessment_window, int duration_assessment_window, int nb_odors, int odors[], int odor_valence[], String odor_name[]){
    // Setup the olfactometer
    setDurationOlfacto(duration_odor_sampling); // send duration to olfactometer
    idleOlfacto(); // put the olfacto in iddle mode
    
    // Send parameters
    Serial.print(String(millis()));
    Serial.print(",BIP,");
    Serial.print(String(current_block));
    Serial.print(",");
    Serial.print(String(nb_trials));
    Serial.print(",");
    Serial.println(String(nb_odors));
    
    Serial.print(String(millis()));
    Serial.print(",BID,");
    Serial.print(String(current_block));
    Serial.print(",");
    Serial.print(String(duration_odor_sampling));
    Serial.print(",");
    Serial.print(String(duration_wait));
    Serial.print(",");
    Serial.print(String(duration_outcome));
    Serial.print(",");
    Serial.println(String(duration_interstimulus_interval));
    
    Serial.print(String(millis()));
    Serial.print(",BIW,");
    Serial.print(String(current_block));
    Serial.print(",");
    Serial.print(String(start_assessment_window));
    Serial.print(",");
    Serial.println(String(duration_assessment_window));
    
    for(int i=0; i<nb_odors; i++){
      Serial.print(String(millis()));
      Serial.print(",BIO,");
      Serial.print(String(current_block));
      Serial.print(",");
      Serial.print(String(i+1));
      Serial.print(",");
      Serial.print(odor_name[i]);
      Serial.print(",");
      Serial.print(String(odor_valence[i]));
      Serial.print(",");
      Serial.println(String(odors[i]));
    }
    
    for (int trial_id=1; trial_id < (nb_trials + 1); trial_id++) {
      Serial.print(String(millis()));
      Serial.print(",O,");
      Serial.print(String(current_block));
      Serial.print(",");
      Serial.print(String(trial_id));
      Serial.print(",");
      Serial.println(String(block_order[trial_id-1]+1));
      
      byte odor_on=1;
      byte outcome_on=0;
      byte outcome_code=odor_valence[block_order[trial_id-1]];
       
      unsigned long start_count_time;
      start_count_time=millis();
      
      byte lastLickState=0;
      byte lickState=0;
      int countLicks=0;
      int state=0;

      stimulusOlfacto(odors[block_order[trial_id-1]]); // start odor delivery from olfactometer
      
      while(state < 4){
        lickState=digitalRead(lickInPin);
        if(lickState != lastLickState){
          if(lickState == HIGH){
            countLicks++;
            Serial.print(String(millis()));
            Serial.print(",L,");
            Serial.print(String(current_block));
            Serial.print(",");
            Serial.print(String(trial_id));
            Serial.print(",");
            Serial.print(String(countLicks));
            Serial.println(",1");
          }
          else{
            Serial.print(String(millis()));
            Serial.print(",L,");
            Serial.print(String(current_block));
            Serial.print(",");
            Serial.print(String(trial_id));
            Serial.print(",");
            Serial.print(String(countLicks));
            Serial.println(",0");
          }
          lastLickState=lickState;
        }
        switch(state)
        {
          case 0:
            if(millis() > (start_count_time + duration_odor_sampling)){
              state=1;
            }
            break;
          case 1:
            if(odor_on == 1){
                //present blank between odorants
                odor_on=0;
            }
            else if(millis() > (start_count_time + duration_odor_sampling + duration_wait)){
              state=2;
            }
            break;
          case 2:
            if((outcome_on == 0) && (outcome_code != 0)){
              switch(outcome_code){
                case 1:
                  deliverWaterReward(reward_solenoid_length);
                  outcome_on=2;
                  break;
                case 2:
                  digitalWrite(punishmentOutPin,HIGH);
                  outcome_on=1;
                  break;
              }
              Serial.print(String(millis()));
              Serial.print(",US,");
              Serial.print(String(current_block));
              Serial.print(",");
              Serial.print(String(trial_id));
              Serial.print(",");
              Serial.print(String(outcome_code));
              Serial.println(",1");
            }
            else if(millis() > (start_count_time + duration_odor_sampling + duration_wait + duration_outcome)){
              state=3;
            }
            break;
          case 3:
            if(outcome_code == 2){
              digitalWrite(punishmentOutPin,LOW);
            }
            state=4;
            break;
          default:
            state=0;
            break;
        }
      }
      
      delay(duration_interstimulus_interval);
    }
}
