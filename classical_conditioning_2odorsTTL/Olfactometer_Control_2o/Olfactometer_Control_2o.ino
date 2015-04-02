

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

int num_valves = 3;
int all_valves[] = {V1, V2, V3};
//String odorants[15] = {"pentanal", "methyl-butanol", "phenetol", "trimethyl-thiazol", "triethylamine", "exanoic", "heptanol", "guaiacol", "benzothiazol", "isoamylamine", "heptanal", "nonanol", "cresol", "methilthiazol", "phenyl-ethylamine"};   
String odorants[2] = {"EUG", "ETHYLACET"};

// =================================
int stimulus_duration = 1000;

int last_odor=0;

// =======================================
// Valve control functions

int current_valve = 1;
void OpenValve(int valveNum) {
  // error checking
  if (digitalRead(all_valves[valveNum-1]) == HIGH) {
    Serial.print(F("Valve ")); Serial.print(valveNum); Serial.println(F("already open."));
    return;
  }
  if ((ONE_VALVE_OPEN) && (valveNum==1)) {
   Serial.println(F("Warning: Valve 1 cannot be directly opened in 'ONE_VALVE_OPEN' mode.")); 
   return;
  }
    
  // open the valve
  digitalWrite(all_valves[valveNum-1], HIGH);
  // close the currenly open valve, if necessary
  if (ONE_VALVE_OPEN) {
    digitalWrite(all_valves[current_valve-1], LOW);
    if (current_valve>1) {
      Serial.print("Valve ");
      Serial.print(current_valve);
      Serial.println(" CLOSED.");
    }
  }
  if (ONE_VALVE_OPEN) {
    current_valve = valveNum;
  }
    Serial.print("   VALVE ");
    Serial.print(valveNum);
    Serial.println(" OPEN.");
 
}

void CloseValve(int valveNum) {
  // error checking
  if (digitalRead(all_valves[valveNum-1]) == LOW) {
    Serial.print(F("Cannot close valve ")); Serial.print(valveNum); Serial.println(F(", it's not open."));
    return;
  }
  if (ONE_VALVE_OPEN && (valveNum==1)) {
   Serial.println(F("Warning: Valve 1 cannot be directly closed in 'ONE_VALVE_OPEN' mode.")); 
   return;
  }

  // close valve  
  digitalWrite(all_valves[valveNum-1], LOW);

  // open valve 1 if necessary
  if (ONE_VALVE_OPEN) {
    current_valve = 1;
    digitalWrite(V1, HIGH);
   Serial.println("-- V1 Open --");
  }
    Serial.print("Valve ");
    Serial.print(valveNum);
    Serial.print(",");
    Serial.println(" CLOSED.");
}


// =======================================
// BNC Triggering Functions
void WaitForTrigger() {
 while (digitalRead(BNC_pin) == LOW) {};
}



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
  
  // Set up pin for BNC input/output
  pinMode(BNC_pin, INPUT);

  // start USB connection with the computer
  Serial.begin(9600);
}


// =================
// === MAIN LOOP ===
// =================





void loop() {
  WaitForTrigger();
  if(last_odor == 2){
    olfStim(3);
    last_odor=3;
  }
  else{
    olfStim(2);
    last_odor=2;
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
        OpenValve(v);
        delay(stimulus_duration);    
        CloseValve(v);
}

void idle() {
  // close valves  
  for (int v=0; v<4; v++) {
    digitalWrite(all_valves[v], LOW);
  }
  // open valve 1 if necessary
  if (ONE_VALVE_OPEN) {
    current_valve = 1;
    digitalWrite(V1, HIGH);
  }
}
    
    
