// User-modifiable global variables:
// =================================

boolean ONE_VALVE_OPEN = true; // When true, exactly one valve is open at a time. Value 1 is open by default.
                               // When false, every value openeing and closing is explicitly controlled by user.
                               
boolean DISPLAY_VALVE_STATUS = true; // Write all valve openings/closing to USB

float CARRIER_FLOW_RATE = 1.0; // default carrier rate in LPM
float ODOR_FLOW_RATE = 0.2; // default carrier rate in LPM

// =================================
int stimulus_duration = 1000; // default stimulus duration

int last_odor=0;

// =======================================
// Valve control functions

int current_valve = 1;

// ==============

#include <SPI.h>
#include "SPI_anything.h"
#include "Olfactometer_header.h"

typedef struct commandStruct
{
  char cmd='Z';
  int param=0;
};

commandStruct rCommand;
float lastCarrierFlowMeasure;
float lastOdorFlowMeasure;

// === SETUP ===
// the setup function runs once when you press reset or power the board
void setup() {
  
  // Set up pins for valve control
  for (int i=0; i<num_valves; i++) {
    pinMode(all_valves[i], OUTPUT);
    digitalWrite(all_valves[i], LOW);
  }
  if (ONE_VALVE_OPEN) {
    digitalWrite(V1, HIGH);
  }
  
  initializeIcspBoard();
  
  // start connections with the MFCs (Serial1 / RS485)
  pinMode(RTS_pin, OUTPUT);
  digitalWrite(RTS_pin, LOW); // RTS low
  Serial1.begin(9600);

  // Setup MFCs
  delay(3000); // wait unlit MFCs are active and ready for input...
  setupMFC(carrier_address, CARRIER_FLOW_RATE); // set carrier stream to 1 LPM
  setupMFC(odor_address, ODOR_FLOW_RATE);   // set odor stream to 0.1 LPM
  
  // start USB connection with the computer
  Serial.begin(9600);
  
  delay(3000);
  displayFlowRates();

}


// =================
// === MAIN LOOP ===
// =================

void loop() {
    if(digitalRead(icspInPin) == HIGH){
     SPI_readAnything(rCommand);
        switch(rCommand.cmd){
          case 'T':
            olfStim(int(rCommand.param),2);
            break;
          case 'O':
            OpenValve(int(rCommand.param));
            break;
          case 'C':
            CloseValve(int(rCommand.param));
            break;
          case 'D':
            stimulus_duration=rCommand.param;
            Serial.print("Duration changed to ");
            Serial.println(String(rCommand.param));
            break;
          case 'M':
            changeMFCFlow(1, (float) rCommand.param/100);
            displayFlowRates();
            break;
          case 'F':
            changeMFCFlow(2, (float) rCommand.param/100);
            displayFlowRates();
            break;
          case 'W':
            changeMFCFlow(1, 2.0);
            changeMFCFlow(2, 0.2);
            displayFlowRates();
            while(1){
              for(int v=2; v<(num_valves+1); v++){
                OpenValve(v);
                delay(3000);
                CloseValve(v);
              }
              delay(15);
            }
            break;
          case 'I':
            idle();
            break;
        }
    }
//  if (Serial.available()) {
//    char ch = Serial.read();
//    if( isDigit(ch) ) {
//      if ( ch == '0' ) {
//        idle();
//      }
//      else if ( ch == '1' ) {
//        olfStim(2);
//        delay(1000);
//        olfStim(3);
//      }
//      else if(ch == '3'){
//        displayFlowRates();
//      }
//    }
//  }
}
