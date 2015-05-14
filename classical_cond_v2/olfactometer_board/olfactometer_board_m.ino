#include <SPI.h>
#include <SPI_anything.h>
#include "Olfactometer_global.h"
#include "MFC_control.h"
#include "icspSlaveFunctions.h"
#include "valves_functions.h"

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
  pinMode(icspOutPin,OUTPUT);
  digitalWrite(icspOutPin,LOW);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

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
    if(digitalRead(SS) == LOW){
    	commandStruct rCommand;
     SPI_readAnything(rCommand);
        switch(rCommand.cmd){
          case 'T':
            olfStim(int(rCommand.param));
            sendFlowInfoToBoard(rCommand.param, (int) lastCarrierFlowMeasure*100, (int) lastOdorFlowMeasure*100);
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
            changeMFCFlow(1, (float) rCommand.param/100);
            displayFlowRates();
            break;
          case 'I':
            idleOlfacto();
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
