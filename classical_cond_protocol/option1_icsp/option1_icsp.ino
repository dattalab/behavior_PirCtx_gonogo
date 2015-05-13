// -- Import required libraries
#include <SPI.h>
#include "SPI_anything.h"
#include "wiring.h"

#include <SD.h>
#include <IniFile.h>
// parameters of TTL & flow
const int pulse_length = 100;
const int reward_solenoid_length=15;
const int punishment_airpuff_duration=500;

// These variables are used to know what program to launch & when
int current_phase=2; // program to launch
byte current_go=0; // set to 0 to wait, 1 to start immediately
int running_state=1; // this is a little indicator to be able to stop/resume/pause the execution
int* pRunningState;
char serial_read;
int statusFlow=0;

            
typedef struct olfactoFeedbackStruct
{
  char cmd;
  int param1;
  int param2;
  int param3;
};

      olfactoFeedbackStruct olfactoFeedback;
      
void setup(){ // initialization

  initializePinStatus();
  initializeIcspOlfacto();
  
  // initialize random number generator seed
  randomSeed(analogRead(randomInPin));
  
  // running state pointer assignment
  pRunningState=&running_state;
  
  // start Serial communication with computer
  Serial.begin(9600);
  Serial.print(millis());
  Serial.println(",CONOK,1");

}


void loop() {
  // This is the main entrance, it actually waits for Serial input to know what program to start and when
  
  if(Serial.available()){
    serial_read=Serial.read();
    switch(serial_read){
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
      case 'G':
        int randomITI[20];
        randITI(randomITI,20,2.0,0.0,25);
        for(int t=0; t<20; t++){
          Serial.println(randomITI[t]);
        }
        break;
      case 'R':
        deliverWaterReward(reward_solenoid_length);
        break;
      case 'W':
        deliverWaterReward(200);
        break;
      case 'T':
        // start odor delivery from olfactometer
        statusFlow=0;
        stimulusOlfacto(2);
        while(statusFlow == 0){
          if(digitalRead(icspInPin) == HIGH){
            digitalWrite(SS,LOW);
            SPI_readAnything (olfactoFeedback);
            digitalWrite(SS,HIGH);
            Serial.print("// ");
            Serial.print(String(millis()));
            Serial.print(",FR,0,");
            Serial.print(String(olfactoFeedback.param1));
            Serial.print(",");
            Serial.print(String(olfactoFeedback.param2/100));
            Serial.print(",");
            Serial.println(String(olfactoFeedback.param3/100));
            statusFlow=1;
           }
          }
         break;
      case 'Z':
        current_go=1;
        break;
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
        opt1_p2_association_training();
        break;
      case 3:
        opt2_p2_association_odor();
        break;
    }
  }
}
