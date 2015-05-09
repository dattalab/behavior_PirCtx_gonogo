#include <SPI.h>
#include "SPI_anything.h"
#include "Olfactometer_header.h"

typedef struct commandStruct
{
  char cmd='Z';
  int param=0;
};

// User-modifiable global variables:
// =================================

boolean ONE_VALVE_OPEN = true; // When true, exactly one valve is open at a time. Value 1 is open by default.
                               // When false, every value openeing and closing is explicitly controlled by user.

float CARRIER_FLOW_RATE = 1.0; // in LPM
float ODOR_FLOW_RATE = 0.2; // in LPM

// =================================
int stimulus_duration = 1000; // default stimulus duration

int last_odor=0;

// =======================================
// Valve control functions

int current_valve = 1;

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
  
    pinMode(BNC_pin,INPUT);
    SPI.begin ();
  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  
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
  Serial.println("CARRIER Flow Rate: ");
  Serial.print(" Set to: ");
  Serial.print(CARRIER_FLOW_RATE);
  Serial.println(" LPM");
  Serial.print(" Actual: ");
  Serial.print(getMFCFlowRate(carrier_address));
  Serial.println(" LPM");
  Serial.println("");

  Serial.println("ODOR Flow Rate: ");
  Serial.print(" Set to: ");
  Serial.print(ODOR_FLOW_RATE);
  Serial.println(" LPM");
  Serial.print(" Actual: ");
  Serial.print(getMFCFlowRate(odor_address));
  Serial.println(" LPM");
  Serial.println("");

}


// =================
// === MAIN LOOP ===
// =================





void loop() {
   if(digitalRead(BNC_pin) == HIGH){
     commandStruct rCommand;
      digitalWrite(SS,LOW);
      SPI_readAnything (rCommand);
      digitalWrite(SS,HIGH);
      switch(rCommand.cmd){
        case 'T':
          olfStim(rCommand.param);
          break;
        case 'O':
          OpenValve(rCommand.param);
          break;
        case 'C':
          CloseValve(rCommand.param);
          break;
        case 'D':
          stimulus_duration=rCommand.param;
          Serial.print("Duration changed to ");
          Serial.println(String(rCommand.param));
          break;
        case 'I':
          idle();
          break;
      }
      Serial.print("CARRIER rate: ");
      Serial.println(getMFCFlowRate(carrier_address));
      Serial.print("ODOR rate: ");
      Serial.println(getMFCFlowRate(odor_address));
      Serial.println("");
    }
  if (Serial.available()) {
    char ch = Serial.read();
    if( isDigit(ch) ) {
      if ( ch == '0' ) {
        idle();
      }
      else if ( ch == '1' ) {
        olfStim(2);
        delay(1000);
        olfStim(3);
      }
      else if(ch == '3'){
        Serial.print("CARRIER rate: ");
        Serial.println(getMFCFlowRate(carrier_address));
        Serial.print("ODOR rate: ");
        Serial.println(getMFCFlowRate(odor_address));
        Serial.println("");
      }
    }
  }
}





void olfStim(int v) {
  if(v < (num_valves+1)){
        OpenValve(v);
        delay(stimulus_duration);    
        CloseValve(v);
  }
}

void idle() {
  // close valves  
  for (int v=0; v<num_valves; v++) {
    digitalWrite(all_valves[v], LOW);
  }
  // open valve 1 if necessary
  if (ONE_VALVE_OPEN) {
    current_valve = 1;
    digitalWrite(V1, HIGH);
  }
}
    
    
