String bufferReadOlfacto;

void initializeOlfacto(){
  olfacto.begin(F("/mnt/sda1/arduino/www/combined_interface.py"));
  olfacto.runAsynchronously();
}

// This is the function used to send a command to the olfactometer
void sendCommandToOlfacto(String myCmd){
  for(int i=0; i < (myCmd.length() + 1); i++){
    olfacto.write(myCmd[i]);
  }
  olfacto.write('\n');
}

// To idle the olfactometer, we send "I,0"
void idleOlfacto(){
  sendCommandToOlfacto(F("I,0"));
}

// To start a new trial, we send "T/X" where X is the number of the valve
void stimulusOlfacto(int v){
  sendCommandToOlfacto((String) F("T,") + v);
}

void updateFlowOlfacto(int mfc, int rate){
  switch(mfc){
    case 0:
      sendCommandToOlfacto((String) F("FCS,") + rate);
      lastRequestedFlows[0]=rate;
    break;
    case 1:
      sendCommandToOlfacto((String) F("FOS,") + rate);
      lastRequestedFlows[1]=rate;
    break;
  }
}

void checkFeedbackOlfacto(){
  while(olfacto.available() > 0){
    char c = char(olfacto.read());
    if(c == '\n'){
      Console.println((String) F("//") + millis() + "," + bufferReadOlfacto);
      bufferReadOlfacto="";
    }
    else if(c != '\r'){
      bufferReadOlfacto+=c;
    }
  }
}

// To set trial duration, we send "D/X" where X is the duration of the stimulus (ms)
void setDurationOlfacto (int duration){
  sendCommandToOlfacto((String) F("D,") + duration);
}
