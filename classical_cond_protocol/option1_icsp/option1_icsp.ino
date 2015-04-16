// -- Import required libraries
#include <elapsedMillis.h>
#include <SPI.h>
#include "SPI_anything.h"

// -- Lickport learning parameters
int lickportphase_successful_required=21; // nb of successful trials required
int lickportphase_consecutive_hits=3;
int lickportphase_delay=10000;
unsigned int lickportphase_timeout=60000;
unsigned int lickportphase_minimum_reward_interval=500;

// -- Associative learning parameters TO CHANGE HERE
// Trial number
int nb_blocks = 3; // nb of blocks to run
int nb_trials_per_block=100; // nb of trials per block
// Durations
int duration_odor_sampling = 1000; // duration of odor sampling
int duration_wait = 1000; // waiting duration
int duration_outcome = 1000; // duration of outcome
int duration_interstimulus_interval = 1000; // duration of ISI
int start_assessment_window=1000; // Start of assessment window
int duration_assessment_window=1000; // Duration of assessment window
// Odors
int odors[]={2,3}; // odor valves (1 is reserved for blank)
int odor_valence[]={1,0}; // odor valences (0 = nothing, 1 = water, 2 = air puff)
String odor_name[]={"EUG","ETHYLACET"}; // odor names
int nb_odors=2; // nb of odors

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

elapsedMillis timer;

typedef struct commandStruct
{
  char cmd;
  int param;
};

commandStruct tCommand;
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
        tube_training();
        break;
      case 2:
        association_training(); //trigger stimuli
        break;
    }
  }
}
