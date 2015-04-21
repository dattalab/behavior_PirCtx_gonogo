// This function opens the solenoid for the specified duration (int, ms)
void deliverWaterReward(int duration_water_reward){
  digitalWrite(solenoidOutPin,HIGH);
  delay(duration_water_reward);
  digitalWrite(solenoidOutPin,LOW);
}

// This function randomizes the elements of an int array
void randomizeArray(int *parray, int nb_elements){
  for (int i= 0; i< nb_elements; i++) {
    int pos = random(nb_elements-1); 
    int temp = parray[i];  
    parray[i] = parray[pos];
    parray[pos] = temp;
  }
}

// This function enables pause/resume/stop of the experiment
// It reads what's in the incoming Serial data
// Codes:
// P: pause, R: resume, S: stop
void checkPauseResume(int *pRunningState){
  int entry=1; // it has to run at least once
  while((pRunningState == 0) || (entry == 1)){ // if execution is paused or for the first time
    if(Serial.available()){
      char rChar=Serial.read();
      switch(rChar){
        case 'P':
          pRunningState=0;
          break;
        case 'R':
          pRunningState=1;
          break;
        case 'S':
          pRunningState=2;
          break;
      }
      entry=0;
    }
  }
}
