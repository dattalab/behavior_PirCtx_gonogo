// This function opens the solenoid for the specified duration (int, ms)
void deliverWaterReward(int duration_water_reward){
  digitalWrite(solenoidOutPin,HIGH);
  delay(duration_water_reward);
  digitalWrite(solenoidOutPin,LOW);
}

// This function randomizes the elements of an int array
void randomizeArray(int *parray, int nb_elements){
  // initialize random number generator seed
  randomSeed(analogRead(randomInPin));
  
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
void checkPauseResume(){
  int *pRunningState=&running_state;
  int entry=1; // it has to run at least once
  
  while((*pRunningState == 0) || (entry == 1)){ // if execution is paused or for the first time
    if(Serial.available()){
      char rChar=Serial.read();
      switch(rChar){
        case 'P':
          *pRunningState=0;
          Serial.print("// ");
          Serial.print(String(millis()));
          Serial.println(", PAUSED");
          break;
        case 'R':
          *pRunningState=1;
          Serial.print("// ");
          Serial.print(String(millis()));
          Serial.println(", RESUMED");
          break;
        case 'S':
          *pRunningState=2;
          Serial.print("// ");
          Serial.print(String(millis()));
          Serial.println(", STOPPED");
          break;
      }
    }
    entry=0;
  }
}

// This function returns randomized ITI with an exponentional distribution (max ITI as defined)
void randITI(int *pRandomizedITI, int nb_trials, float lambda, float mean, int max_ITI){
  // initialize random number generator seed
  randomSeed(analogRead(randomInPin));
  
  for(int t=0; t<nb_trials; t++){
    pRandomizedITI[t]=round((mean+(log((random(0,10000)/10000.0)/lambda))*10/(-lambda))*1000.0);
    if(pRandomizedITI[t] > max_ITI*1000){
      pRandomizedITI[t]=max_ITI*1000;
    }
  }
}
