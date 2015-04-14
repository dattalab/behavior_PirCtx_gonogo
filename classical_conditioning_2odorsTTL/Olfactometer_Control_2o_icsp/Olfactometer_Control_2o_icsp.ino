#include <SPI.h>
#include "SPI_anything.h"

typedef struct commandStruct
{
  char cmd='Z';
  int param=0;
};

// User-modifiable global variables:
// =================================

boolean ONE_VALVE_OPEN = true; // When true, exactly one valve is open at a time. Value 1 is open by default.
                               // When false, every value openeing and closing is explicitly controlled by user.


// =================================
// Pin Assignments
int BNC_pin = 2;
int V1 = 3;
int V2 = 4;
int V3 = 5;
int V4 = 6;
int V5 = 7;
int V6 = 8;
int V7 = 9;
int V8 = 10;
int V9 = 11;
int V10 = 12;
int V11 = 13;
int V12 = A0;
int V13 = A1;
int V14 = A2;
int V15 = A3;
int V16 = A4;

int num_valves = 16;
int all_valves[] = {V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16};

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

  // start USB connection with the computer
  Serial.begin(9600);
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
//    }
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
    
    
