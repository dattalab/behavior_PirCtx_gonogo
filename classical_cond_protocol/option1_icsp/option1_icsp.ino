// -- Import required libraries
#include <elapsedMillis.h>
#include <SPI.h>
#include "SPI_anything.h"

// Wiring information
const byte buttonInPin = 2; // Push button
const byte LED1OutPin = 6; // Green LED
const byte LED2OutPin = 7; // Red LED
const byte icspOutPin = 8; // ICSP Communication Pin
const byte lickInPin = 3; // Capacitor
const byte solenoidOutPin = 4;
const byte punishmentOutPin = 5; // Airpuff
int randomInPin = 11; // Analog input for random seed generation

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
int randomITI[20];

void setup(){ // initialization

  // set pin status
  pinMode(randomInPin,INPUT);
  pinMode(buttonInPin,INPUT);
  pinMode(LED1OutPin,OUTPUT);
  pinMode(LED2OutPin,OUTPUT);
  pinMode(icspOutPin,OUTPUT);
  pinMode(lickInPin,INPUT);
  pinMode(solenoidOutPin,OUTPUT);
  pinMode(punishmentOutPin,OUTPUT);
  
  digitalWrite(LED1OutPin,LOW);
  digitalWrite(LED2OutPin,LOW);
  digitalWrite(solenoidOutPin,LOW);
  digitalWrite(punishmentOutPin,LOW);
  
  // ICSP communication initialization with olfactometer (the board acts as a slave)
  pinMode(MISO, OUTPUT);
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
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
