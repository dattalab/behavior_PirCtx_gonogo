#include <SPI.h>
#include <Ethernet.h>
#include <elapsedMillis.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x97, 0xE2};
byte ip[] = {192,168,20,18};
byte server[] = {192,168,20,85};
int port = 3336;

byte trigger = 7;
byte TTL=6;
int pulse_length = 100;

String carrierRate = "135";
String odorRate = "1000";
String mfc3 = odorRate;
String mfc2 = odorRate;

String mfc5 = carrierRate;
String mfc6 = carrierRate;

int number_of_trials = 10;
int stimulus_duration = 1000;
int inter_stimulus_interval = 1000; 
 
int active_valves = 24;
int begin_odor=0;
int end_trial=0;

EthernetClient client;

void setup() { 
  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  delay(3000);
  int connexion = -1;
  while(connexion != 1){
    connexion=client.connect(server, port);
    if(connexion < 0){
      Serial.println("// Connection failed!");
      delay(500);
    }
    else { Serial.println("Connected!");
    
    }
  }
   
   // set MFC and Valve Flow Rates
client.print("write BankFlow3_Actuator ");
client.println(mfc3);
Serial.print("write BankFlow3_Actuator ");
Serial.println(mfc3);
 client.print("write BankFlow2_Actuator ");
 client.println(mfc2);
 Serial.print("write BankFlow2_Actuator ");
 Serial.println(mfc2);
 
 client.print("write Carrier1_Actuator ");
 client.println(mfc5);
 client.print("write Carrier2_Actuator ");
 client.println(mfc6);
 Serial.print("write Carrier1_Actuator ");
 Serial.println(mfc5);
 Serial.print("write Carrier2_Actuator ");
 Serial.println(mfc6);
 
 client.println("write Bank2_Valves 0");
 client.println("write Bank3_Valves 0");

  pinMode(trigger, OUTPUT);  
  digitalWrite(trigger, LOW); 
  pinMode(TTL, OUTPUT);  
  digitalWrite(TTL, LOW);  
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
  Serial.println("shit ");
}
  
void olfStim() {

  for (int trial = 1; trial < number_of_trials + 1; trial++) {
  if(Serial.available()){
     char chara=Serial.read();
     if(chara == '0'){
     break; //trigger stimul
    }
     }
    Serial.print("trial ");
    Serial.println(trial);
    // trigger for logging next file in scanimage5.
      begin_odor=millis();
      digitalWrite(trigger, HIGH); 
      delay(pulse_length);
      digitalWrite(trigger, LOW);
      delay(inter_stimulus_interval);
      
      
    for (int odor = 1; odor < (active_valves+1); odor++) {
      
      if(Serial.available()){
     char chara=Serial.read();
     if(chara == '0'){
     break; //trigger stimul
    }
     } 
      //deliver odorant
      digitalWrite(TTL, HIGH);
      
      if(odor < 16){
        client.print("write Bank2_Valves ");
        client.println(String(odor));
        Serial.print("write Bank2_Valves ");
        Serial.println(String(odor));
      }
      else if(odor < 24){
        client.print("write Bank3_Valves ");
        client.println(String(odor-15));
        Serial.print("write Bank3_Valves ");
        Serial.println(String(odor-15));
        }
        
      delay(stimulus_duration);
      digitalWrite(TTL, LOW);
      
     if(odor < 16){
         client.println("write Bank2_Valves 0");
         Serial.println("write Bank2_Valves 0");
     }
     else if(odor < 25){
       client.println("write Bank3_Valves 0");
       Serial.println("write Bank3_Valves 0");
     }

     delay(inter_stimulus_interval);
     
     
    }
    end_trial=millis();
    delay(20);
    Serial.println(String(end_trial-begin_odor));
  }
}


