#include <elapsedMillis.h>
#include <SPI.h>
#include <Ethernet.h>
#include "olfactometer_control.h"

// Trial parameters
int nb_blocks = 1;
int nb_trials_per_odor = 5;
int nb_odors = 24;
int stimulus_duration = 1000;
int inter_stimulus_interval = 1000;
int max_consecutive = 3;

// Pin assignments
byte trigger = 7;
byte TTL=6;
int randomInPin = 8;

// TTL parameters
int pulse_length = 100;

// Olfactometer airflow parameters
int carrierRate=135;
int odorRate=1000;

void setup() { 
  Serial.begin(9600);
  //olfactoConnexion();
  
   // set MFC and Valve Flow Rates
 setBankFlow(2,odorRate);
 setBankFlow(3,odorRate);
 
 setCarrierFlow(1,carrierRate);
 setCarrierFlow(2,carrierRate);
 
 blankBank(2);
 blankBank(3);

  pinMode(trigger, OUTPUT);  
  digitalWrite(trigger, LOW); 
  pinMode(TTL, OUTPUT);  
  digitalWrite(TTL, LOW);
  pinMode(randomInPin,INPUT);
}


void loop() {
  if(Serial.available()){
    char chara=Serial.read();
    if(chara == '1'){
      olfStim(); //trigger sweep
    }
    if(chara == '0'){
      chill(); 
    }
  }
    
}


void chill() {
   blankBank(2);
   blankBank(3);
   Serial.println("// shit");
}
  
void olfStim() {
  for (int block_id = 1; block_id < (nb_blocks+1); block_id++) {
   if(Serial.available()){
       char chara=Serial.read();
       if(chara == '0'){
         break; //trigger stimul
        }
    }
    Serial.print("// BLOCK ID: ");
    Serial.println(block_id);
    int block_sequence[nb_trials_per_odor*nb_odors];
    int seq_ok = 0;
    while(seq_ok == 0){
      Serial.println("Generating sequence...");
      randomSequence(block_sequence,nb_odors,nb_trials_per_odor,max_consecutive);
      
      for(int s=0; s < nb_trials_per_odor*nb_odors; s++){
        Serial.print(String(block_sequence[s]));
        Serial.print("    ");
        if(s%10 == 0){
          Serial.println("");
        }
      }
      
      while(Serial.available() == false){}
      
      if(Serial.read() == '1'){
         seq_ok=0;
      }
    }
    // trigger for logging next file in scanimage5.
    int block_start=millis();
    sendTriggerTTL();
    
    delay(inter_stimulus_interval);
      
    for (int trial_id = 0; trial_id < (nb_trials_per_odor*nb_odors); trial_id++) {
      if(Serial.available()){
       char chara=Serial.read();
       if(chara == '0'){
       break; //trigger stimul
      }
     } 
     
     //deliver odorant
     digitalWrite(TTL, HIGH);
     openValve(block_sequence[trial_id]);
         Serial.print(String(millis()));
    Serial.print(",");
    Serial.print(String(block_sequence[trial_id]));
    Serial.print(",");
     delay(stimulus_duration);
     digitalWrite(TTL, LOW);
      
     if(block_sequence[trial_id] < 16){
        blankBank(2);
     }
     else if(block_sequence[trial_id] < 25){
        blankBank(3);
     }
    
    Serial.println(String(millis()));

     delay(inter_stimulus_interval);

    }
    int block_end=millis();
    
    Serial.println(String(block_start-block_end));
  }
}


