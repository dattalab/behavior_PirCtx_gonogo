void p1_tube_training() {

  // -- Lickport learning parameters
  int lickportphase_successful_required = 21; // nb of successful trials required
  int lickportphase_consecutive_hits = 3;
  int lickportphase_delay = 10000;
  unsigned int lickportphase_timeout = 60000;
  unsigned int lickportphase_minimum_reward_interval = 500;
  // -- End of parameters

  int nb_successful_trials = 0;
  int nb_consecutive_successful_trials = 0;
  unsigned long start_count_time;
  byte lastLickState = 0;
  byte lickState = 0;
  int countLicks = 0;
  int current_block = 0;
  unsigned long time_last_reward = 0;

  idleOlfacto();

  Serial.println((String) F("// ") + millis() + ",PHASE,1" + millis() + F(",TLIP,") + String(lickportphase_successful_required) + "," + String(lickportphase_consecutive_hits) + millis() + F(",TLID,") + String(lickportphase_delay) + "," + String(lickportphase_timeout));

  while (nb_successful_trials < lickportphase_successful_required) {
    digitalWrite(LED1OutPin, HIGH);
    digitalWrite(LED2OutPin, HIGH);
    while (digitalRead(buttonInPin) != HIGH) {};
    digitalWrite(LED1OutPin, LOW);
    digitalWrite(LED2OutPin, LOW);
    current_block++;
    nb_consecutive_successful_trials = 0;
    start_count_time = millis();
    lastLickState = 0;
    countLicks = 0;
    time_last_reward = 0;
    Serial.println((String)millis() + F(",TLB,") + current_block);
    while ((nb_consecutive_successful_trials < lickportphase_consecutive_hits) && (nb_successful_trials < lickportphase_successful_required)) {
      lickState = digitalRead(lickInPin);
      if (lickState != lastLickState) {
        if (lickState == HIGH) {
          countLicks++;
          start_count_time = millis();
          Serial.println((String) millis() + F(",L,") + String(current_block) + "," + String(nb_successful_trials) + "," + String(countLicks) + F(",1"));
          if (millis() > (time_last_reward + lickportphase_minimum_reward_interval)) {
            nb_consecutive_successful_trials++;
            nb_successful_trials++;
            time_last_reward = millis();
            digitalWrite(solenoidOutPin, HIGH);
            delay(reward_solenoid_length);
            digitalWrite(solenoidOutPin, LOW);
            Serial.println((String) millis() + F(",US,0,") + String(nb_successful_trials) + F(",1,1"));
          }
        }
        else {
          Serial.println((String) millis() + F(",L,") + String(current_block) + "," + String(nb_successful_trials) + "," + String(countLicks) + F(",0"));
        }
        lastLickState = lickState;
      }
      if (millis() > (start_count_time + lickportphase_timeout)) {
        Serial.println((String) millis() + F(",TIMEOUT,1"));
        digitalWrite(LED2OutPin, HIGH);
        while (digitalRead(buttonInPin) != HIGH) {};
        digitalWrite(LED2OutPin, LOW);
        start_count_time = millis();
        time_last_reward = millis();
        digitalWrite(solenoidOutPin, HIGH);
        delay(reward_solenoid_length);
        digitalWrite(solenoidOutPin, LOW);
        Serial.println((String) millis() + F(",US,0,0,1,1"));
        lastLickState = 0;
      }
      checkPauseResume();
      if (*pRunningState == 2) {
        nb_successful_trials = lickportphase_successful_required;
      }
    }
    Serial.println((String) millis() + F(",WAIT_BUTTON"));
    digitalWrite(LED1OutPin, HIGH);
    delay(lickportphase_delay);
  }
  digitalWrite(LED1OutPin, LOW);
  digitalWrite(LED2OutPin, LOW);
  Serial.println(F("KILL"));
}
