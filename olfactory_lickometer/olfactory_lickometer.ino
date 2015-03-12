#include <SPI.h>
#include <Ethernet.h>
#include <elapsedMillis.h>

int nb_blocks = 2;
int nb_trials_per_block=3;
int duration_odor_sampling = 1000;
int duration_wait = 1000;
int duration_outcome = 1000;
int duration_interstimulus_interval = 1000;
int odors[]={1,2}; // odor valves (9 is reserved for blank)
int odor_valence[]={1,0};
String odor_name[]={"HEX","HEPT"};
int nb_odors=2;

// Ethernet connexion to olfactometer
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x97, 0xE2};
byte ip[] = {192,168,20,16};
byte server[] = {192,168,20,85};
int port = 3336;
EthernetClient client;

// wiring info
const byte buttonInPin = 2;
const byte triggerOutPin = 13;
const byte lickInPin = 3;
const byte solenoidOutPin = 5;
const byte punishmentOutPin = 6;

// parameters of TTL & flow
const int pulse_length = 100;
const int solenoid_length = 25;
const int reward_solenoid_length=25;
const int punishment_airpuff_duration=500;
String carrierRate = "1000";
String odorRate = "100";
//String mfc3 = odorRate;
//String mfc4 = odorRate;
String mfc1 = odorRate;
String mfc2 = odorRate;
//String mfc7 = carrierRate;
//String mfc8 = carrierRate;
String mfc5 = carrierRate;
String mfc6 = carrierRate;

elapsedMillis timer;

void setup() { 
  Serial.begin(9600);
  Ethernet.begin(mac,ip); // Ethernet client initialization
  delay(1000);
  Serial.println("// connecting...");
  int connexion = 0;
  while(connexion == 0){
    connexion=client.connect(server, port);
    Serial.println("// Connexion failed!");
    delay(500);
  }
  if (connexion == 1) { // Ethernet connexion to the olfactometer
     Serial.println("// connected!");
     // set MFC and Valve Flow Rates
     client.print("write BankFlow1_Actuator ");
     client.println(mfc1);
     client.print("write BankFlow2_Actuator ");
     client.println(mfc2);
     Serial.print("// write BankFlow1_Actuator ");
     Serial.println(mfc1);
     Serial.print("// write BankFlow2_Actuator ");
     Serial.println(mfc2);
     
     client.print("// write Carrier5_Actuator ");
     client.println(mfc5);
     client.print("// write Carrier6_Actuator ");
     client.println(mfc6);
     Serial.print("// write Carrier5_Actuator ");
     Serial.println(mfc5);
     Serial.print("// write Carrier6_Actuator ");
     Serial.println(mfc6);
   } 
   else {
    Serial.println("// Connection failed!");
  }
  
  pinMode(buttonInPin, INPUT); 
  pinMode(triggerOutPin, OUTPUT);
  pinMode(lickInPin, INPUT);
  pinMode(solenoidOutPin,OUTPUT);
  pinMode(punishmentOutPin,OUTPUT);
  
  digitalWrite(triggerOutPin, LOW);
  digitalWrite(solenoidOutPin, LOW);
  digitalWrite(punishmentOutPin, LOW);

}


void loop() {
  int state = digitalRead(buttonInPin);
  if (state == HIGH) {
    olfStim(); //trigger stimuli
  }
}

// TTL for triggering Intan recording
void sendTrigger() {
  digitalWrite(triggerOutPin, HIGH); 
  delay(pulse_length); 
  digitalWrite(triggerOutPin, LOW);
}


void olfStim() {
    
  for (int current_block = 1; current_block < (nb_blocks + 1); current_block++) {
    randomSeed(micros());
    int order[nb_trials_per_block];
    for(int i = 0; i < nb_trials_per_block; i++){
      order[i]=random(1,nb_odors+1);
    }
    Serial.print(String(millis()));
    Serial.print(",BIP,");
    Serial.print(String(current_block));
    Serial.print(",");
    Serial.print(String(nb_trials_per_block));
    Serial.print(",");
    Serial.println(String(nb_odors));
    
    Serial.print(String(millis()));
    Serial.print(",BID,");
    Serial.print(String(current_block));
    Serial.print(",");
    Serial.print(String(duration_odor_sampling));
    Serial.print(",");
    Serial.print(String(duration_wait));
    Serial.print(",");
    Serial.print(String(duration_outcome));
    Serial.print(",");
    Serial.println(String(duration_interstimulus_interval));
    
    for(int i=0; i<nb_odors; i++){
      Serial.print(String(millis()));
      Serial.print(",BIO,");
      Serial.print(String(current_block));
      Serial.print(",");
      Serial.print(String(i+1));
      Serial.print(",");
      Serial.print(odor_name[i]);
      Serial.print(",");
      Serial.print(String(odor_valence[i]));
      Serial.print(",");
      Serial.println(String(odors[i]));
    }
    
    for (int current_odor=0; current_odor < nb_trials_per_block; current_odor++) {
      Serial.print(String(millis()));
      Serial.print(",O,");
      Serial.print(String(current_block));
      Serial.print(",");
      Serial.print(String(current_odor+1));
      Serial.print(",");
      Serial.println(String(order[current_odor]));
      
      //deliver odorant
      byte odor_on=1;
      byte outcome_on=0;
      byte outcome_code=odor_valence[order[current_odor]];
      byte outcome_pin;
      if(outcome_code == 2){
        outcome_pin = punishmentOutPin;
      }
      else{
        outcome_pin = solenoidOutPin;
      }
       
      unsigned long start_count_time;
      start_count_time=millis();
      byte lastLickState=0;
      byte lickState=0;
      int countLicks=0;
      int state=0;
      
      client.print("write Bank2_Valves ");
      client.println(String(odors[order[current_odor] - 1]));
      
      while(state < 4){
        lickState=digitalRead(lickInPin);
        if(lickState != lastLickState){
          if(lickState == HIGH){
            countLicks++;
            Serial.print(String(millis()));
            Serial.print(",L,");
            Serial.print(String(current_block));
            Serial.print(",");
            Serial.print(String(current_odor+1));
            Serial.print(",");
            Serial.print(String(countLicks));
            Serial.println(",1");
          }
          else{
            Serial.print(String(millis()));
            Serial.print(",L,");
            Serial.print(String(current_block));
            Serial.print(",");
            Serial.print(String(current_odor+1));
            Serial.print(",");
            Serial.print(String(countLicks));
            Serial.println(",0");
          }
          lastLickState=lickState;
        }
        switch(state)
        {
          case 0:
            if(millis() > (start_count_time + duration_odor_sampling)){
              state=1;
            }
            break;
          case 1:
            if(odor_on == 1){
                //present blank between odorants
               client.println("write Bank2_Valves 0");
               odor_on=0;
            }
            else if(millis() > (start_count_time + duration_odor_sampling + duration_wait)){
              state=2;
            }
            break;
          case 2:
            if(outcome_on == 0){
              digitalWrite(outcome_pin,HIGH);
              outcome_on=1;
            }
            else if(millis() > (start_count_time + duration_odor_sampling + duration_wait + duration_outcome)){
              state=3;
            }
            else if((outcome_code == 1) && (millis() > (start_count_time + duration_odor_sampling + duration_wait + solenoid_length))){
              digitalWrite(solenoidOutPin,LOW);
            }
            break;
          case 3:
            digitalWrite(outcome_pin,LOW);
            state=4;
            break;
          default:
            state=0;
            break;
        }
      }
      
      delay(duration_interstimulus_interval);
    }
}
Serial.println("KILL");
}

