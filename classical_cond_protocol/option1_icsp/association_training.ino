void association_training() {
    Serial.print("// ");
    Serial.print(String(millis()));
    Serial.println(",PHASE,2");
    
  for (int current_block = 1; current_block < (nb_blocks + 1); current_block++) {
    int order[nb_trials_per_block];
    for(int i = 0; i < nb_trials_per_block; i++){
      order[i]=i%2; 
    }
    
    // send duration to olfactometer
    setDurationOlfacto(duration_odor_sampling);
    
    // put the olfacto in iddle mode
    idleOlfacto();
    
    
    Serial.print(String(millis()));
    Serial.print(",BIP,");
    Serial.print(String(current_block));
    Serial.print(",");
    Serial.print(String(nb_trials_per_block));
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
    
    for (int current_odor=0; current_odor < nb_trials_per_block; current_odor++) {
      Serial.print(String(millis()));
      Serial.print(",O,");
      Serial.print(String(current_block));
      Serial.print(",");
      Serial.print(String(current_odor+1));
      Serial.print(",");
      Serial.println(String(order[current_odor]));
      
      //deliver odorant
      byte odor_on=1;
      byte outcome_on=0;
      byte outcome_code=odor_valence[order[current_odor]];
      byte outcome_pin;
      if(outcome_code == 2){
        outcome_pin = punishmentOutPin;
      }
      else{
        outcome_pin = solenoidOutPin;
      }
       
      unsigned long start_count_time;
      start_count_time=millis();
      byte lastLickState=0;
      byte lickState=0;
      int countLicks=0;
      int state=0;

      stimulusOlfacto(odors[order[current_odor]]);
      
      while(state < 4){
        lickState=digitalRead(lickInPin);
        if(lickState != lastLickState){
          if(lickState == HIGH){
            countLicks++;
            Serial.print(String(millis()));
            Serial.print(",L,");
            Serial.print(String(current_block));
            Serial.print(",");
            Serial.print(String(current_odor+1));
            Serial.print(",");
            Serial.print(String(countLicks));
            Serial.println(",1");
          }
          else{
            Serial.print(String(millis()));
            Serial.print(",L,");
            Serial.print(String(current_block));
            Serial.print(",");
            Serial.print(String(current_odor+1));
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
              digitalWrite(outcome_pin,HIGH);
              outcome_on=1;
              if(outcome_code == 1){
                delay(reward_solenoid_length);
                digitalWrite(solenoidOutPin,LOW);
                outcome_on=2;
              }
              Serial.print(String(millis()));
              Serial.print(",US,");
              Serial.print(String(current_block));
              Serial.print(",");
              Serial.print(String(current_odor+1));
              Serial.print(",");
              Serial.print(String(outcome_code));
              Serial.println(",1");
            }
            else if(millis() > (start_count_time + duration_odor_sampling + duration_wait + duration_outcome)){
              state=3;
            }
            break;
          case 3:
            digitalWrite(outcome_pin,LOW);
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
Serial.println("KILL");
}
