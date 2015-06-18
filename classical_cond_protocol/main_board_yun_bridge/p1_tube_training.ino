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

  writeOut(fstringF(F("//%lu,PHASE,1"),millis()));
  writeOut(fstringF(F("//%lu,TLIP,%i,%i"),millis(),lickportphase_successful_required,lickportphase_consecutive_hits));
  writeOut(fstringF(F("//%lu,TLID,%i,%i"),millis(),lickportphase_delay,lickportphase_timeout));

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
    writeOut(fstringF(F("%lu,TLB,%i"),millis(),current_block));
    while ((nb_consecutive_successful_trials < lickportphase_consecutive_hits) && (nb_successful_trials < lickportphase_successful_required)) {
      lickState = digitalRead(lickInPin);
      if (lickState != lastLickState) {
        if (lickState == HIGH) {
          countLicks++;
          start_count_time = millis();
          writeOut(fstringF(F("%lu,L,%i,%i,%i,1"),millis(),current_block,nb_successful_trials,countLicks));
          if (millis() > (time_last_reward + lickportphase_minimum_reward_interval)) {
            nb_consecutive_successful_trials++;
            nb_successful_trials++;
            time_last_reward = millis();
            digitalWrite(solenoidOutPin, HIGH);
            delay(reward_solenoid_length);
            digitalWrite(solenoidOutPin, LOW);
            writeOut(fstringF(F("%lu,US,0,%i,1,1"),millis(),nb_successful_trials));
          }
        }
        else {
          writeOut(fstringF(F("%lu,L,%i,%i,%i,0"),millis(),current_block,nb_successful_trials,countLicks));
        }
        lastLickState = lickState;
      }
      if (millis() > (start_count_time + lickportphase_timeout)) {
        writeOut(fstringF(F("%lu,TIMEOUT,1"),millis()));
        digitalWrite(LED2OutPin, HIGH);
        while (digitalRead(buttonInPin) != HIGH) {};
        digitalWrite(LED2OutPin, LOW);
        start_count_time = millis();
        time_last_reward = millis();
        digitalWrite(solenoidOutPin, HIGH);
        delay(reward_solenoid_length);
        digitalWrite(solenoidOutPin, LOW);
        writeOut(fstringF(F("%lu,US,0,0,1,1"),millis()));
        lastLickState = 0;
      }
      checkPauseResume();
      if (*pRunningState == 2) {
        nb_successful_trials = lickportphase_successful_required;
      }
    }
    writeOut(fstringF(F("%i,WAIT_BUTTON"),millis()));
    digitalWrite(LED1OutPin, HIGH);
    delay(lickportphase_delay);
  }
  digitalWrite(LED1OutPin, LOW);
  digitalWrite(LED2OutPin, LOW);
  writeOut(F("KILL"));
}
