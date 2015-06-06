// -- Import required libraries
#include <Bridge.h>
#include "wiring.h"

// parameters of TTL & flow
const int pulse_length = 100;
const int reward_solenoid_length=8;
const int punishment_airpuff_duration=500;

// These variables are used to know what program to launch & when
int current_phase=2; // program to launch
byte current_go=0; // set to 0 to wait, 1 to start immediately
int running_state=1; // this is a little indicator to be able to stop/resume/pause the execution
int* pRunningState;
char serial_read;
int lastRequestedFlows[2];

Process olfacto;
Process get_conf;

void setup(){ // initialization
  initializePinStatus();
  Serial.begin(115200);
  
  Bridge.begin();
  
  initializeOlfacto();
  
  // initialize random number generator seed
  randomSeed(analogRead(randomInPin));
  
  // running state pointer assignment
  pRunningState=&running_state;
  
  // start Serial communication with computer
  Serial.println((String) millis() + F(",CONOK,1"));

}

String last_command="";
String id_config="";
String id_step="";

void processCommand(String command){
  char cmd=getValue(command,',',0)[0];
  switch(cmd){
    case 'A':
      current_phase=1;
      current_go=1;
      break;
    case 'B':
      current_phase=2;
      break;
    case 'C':
      current_phase=3;
      break;
    case 'P':
      id_config=getValue(command,',',1);
      id_step=getValue(command,',',2);
      break;
    case 'R':
      deliverWaterReward(reward_solenoid_length);
      break;
    case 'F':
      idleOlfacto();
      updateFlowOlfacto(0,2000);
      break;
    case 'W':
      deliverWaterReward(200);
      break;
    case 'T':
      // start odor delivery from olfactometer
      updateFlowOlfacto(0,1000);
      updateFlowOlfacto(1,100);
      stimulusOlfacto(2);
      break;
    case 'X':
        Serial.println(F("// Loading configuration in the Bridge..."));
        get_conf.begin(F("/mnt/sda1/arduino/www/catch_param_uno2.py"));
        get_conf.addParameter(id_config);
        get_conf.addParameter(id_step);
        get_conf.run();
        Serial.println(F("// Configuration loaded in the Bridge."));
      break;
    case 'Z':
      current_go=1;
      break;
  }
}
void loop() {
  // This is the main entrance, it actually waits for Serial input to know what program to start and when
  checkFeedbackOlfacto();
  while(Serial.available()){
    char c=Serial.read();
    if(c == '\n'){
       processCommand(last_command);
       last_command="";
    }
    else if(c != '\r'){
      last_command+=c;
    }
  }
  
  // if a start command has been received or if the push button has been pressed, start the program
  if(current_go == 1 || digitalRead(buttonInPin) == HIGH){
    current_go=0;
    running_state=1;
    switch(current_phase){
      case 1:
        p1_tube_training();
        break;
      case 2:
        if(id_config.length() != 0){
          startSessionCfg(id_config,id_step);
          break;
        }
        else{
          Serial.println(F("// FATAL ERROR: Undefined configuration."));
        }
    }
  }
}
