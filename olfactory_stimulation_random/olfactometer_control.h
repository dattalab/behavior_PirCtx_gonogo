#include "Arduino.h"

byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x97, 0xE2};
byte ip[] = {192,168,20,18};
byte server[] = {192,168,20,85};
int port = 3336;

EthernetClient client;

void olfactoConnexion(){
  Ethernet.begin(mac,ip);
  delay(3000);
  int connexion = -1;
  while(connexion != 1){
    connexion=client.connect(server, port);
    if(connexion < 0){
      Serial.println("// Connection failed!");
      delay(500);
    }
    else{
      Serial.println("// Connected!");
    }
  }
}

void openValve(int valve){
   if(valve < 16){
        client.print("write Bank2_Valves ");
        client.println(String(valve));
        Serial.print("// write Bank2_Valves ");
        Serial.println(String(valve));
   }
   else if(valve < 24){
     client.print("write Bank3_Valves ");
     client.println(String(valve-15));
     Serial.print("// write Bank3_Valves ");
     Serial.println(String(valve-15));
   }
}

void blankBank(int bank){
     client.print("write Bank");
     client.print(String(bank));
     client.println("_Valves 0");
     Serial.print("// write Bank");
     Serial.print(String(bank));
     Serial.println("_Valves 0");
}

void setCarrierFlow(int carrier, int flow){
   client.print("write Carrier");
   client.print(String(carrier));
   client.print("_Actuator ");
   client.println(String(flow));
   Serial.print("write Carrier");
   Serial.print(String(carrier));
   Serial.print("_Actuator ");
   Serial.println(String(flow));
}

void setBankFlow(int bank, int flow){
  client.print("write BankFlow");
  client.print(String(bank));
  client.print("_Actuator ");
  client.println(String(flow));
  Serial.print("write BankFlow");
  Serial.print(String(bank));
  Serial.print("_Actuator ");
  Serial.println(String(flow));
}
