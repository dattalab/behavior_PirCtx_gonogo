// -- Import required libraries
#include <elapsedMillis.h>
#include <SPI.h>
#include "SPI_anything.h"

// wiring info
const byte buttonInPin = 2;
const byte LED1OutPin = 6;
const byte LED2OutPin = 7;
const byte icspOutPin = 8;
const byte lickInPin = 3;
const byte solenoidOutPin = 4;
const byte punishmentOutPin = 5;

// parameters of TTL & flow
const int pulse_length = 100;
const int reward_solenoid_length=15;
const int punishment_airpuff_duration=500;

int current_phase=2;
byte current_go=0;
char serial_read;

void setup() { 
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
  
  pinMode(MISO, OUTPUT);
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  Serial.begin(9600);
  
  Serial.print(millis());
  Serial.println(",CONOK,1");

}


void loop() {
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
      case 'W':
        init_water();
        break;
      case 'Z':
        current_go=1;
        break;
    }
  }
  
  if(current_go == 1 || digitalRead(buttonInPin) == HIGH){
    current_go=0;
    switch(current_phase){
      case 1:
        p1_tube_training();
        break;
      case 2:
        opt1_p2_association_training(); //trigger stimuli
        break;
    }
  }
}
