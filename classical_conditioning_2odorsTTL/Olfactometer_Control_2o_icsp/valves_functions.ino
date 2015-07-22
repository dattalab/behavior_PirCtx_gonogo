// =======================================
// Valve control functions

void OpenValve(int valveNum) {
  // error checking
  if (digitalRead(all_valves[valveNum-1]) == HIGH) {
    Serial.print("Valve "); Serial.print(valveNum); Serial.println("already open.");
    return;
  }
  if ((ONE_VALVE_OPEN) && (valveNum==1)) {
   Serial.println("Warning: Valve 1 cannot be directly opened in 'ONE_VALVE_OPEN' mode.");
   return;
  }

  // open the valve
  digitalWrite(all_valves[valveNum-1], HIGH);
  // close the currenly open valve, if necessary
  if (ONE_VALVE_OPEN) {
    digitalWrite(all_valves[current_valve-1], LOW);
//    if (current_valve==1) {
//            Serial.println("-- V1 Closed --");
//    }
    if (DISPLAY_VALVE_STATUS && (current_valve>1)) {
      Serial.print(".Valve ");
      Serial.print(current_valve);
      Serial.println(" CLOSED.");
    }
  }
  if (ONE_VALVE_OPEN) {
    current_valve = valveNum;
  }
  if (DISPLAY_VALVE_STATUS) {
    Serial.print("Valve ");
    Serial.print(valveNum);
    Serial.println(" OPEN.");
  }
  
 measureFlow();
}

void CloseValve(int valveNum) {
  // error checking
  if (digitalRead(all_valves[valveNum-1]) == LOW) {
    Serial.print("Cannot close valve "); Serial.print(valveNum); Serial.println(", it's not open.");
    return;
  }
  if (ONE_VALVE_OPEN && (valveNum==1)) {
   Serial.println("Warning: Valve 1 cannot be directly closed in 'ONE_VALVE_OPEN' mode.");
   return;
  }

  // close valve
  digitalWrite(all_valves[valveNum-1], LOW);

  // open valve 1 if necessary
  if (ONE_VALVE_OPEN) {
    current_valve = 1;
    digitalWrite(V1, HIGH);
//    Serial.println("-- V1 Open --");
  }
  if (DISPLAY_VALVE_STATUS) {
    Serial.print("Valve ");
    Serial.print(valveNum);
    Serial.println(" CLOSED.");
  }
}

void olfStim(int v, int mode) {
  if(v < (num_valves+1)){
    OpenValve(v);
    delay(stimulus_duration);    
    CloseValve(v);
    if(mode == 2){
      //sendFlowInfoToBoard(v, int(lastCarrierFlowMeasure*100), int(lastOdorFlowMeasure*100));
    }
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

void displayFlowRates(){
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

void changeMFCFlow(int type, float flow){
  switch(type){ // carrier
    case 1:
      CARRIER_FLOW_RATE=flow;
      setMFCFlow(carrier_address,flow);
      break;
    case 2:
      ODOR_FLOW_RATE=flow;
      setMFCFlow(odor_address,flow);
      break;
  }
  
}

void measureFlow(){
  lastCarrierFlowMeasure=getMFCFlowRate(carrier_address);
  lastOdorFlowMeasure=getMFCFlowRate(odor_address);
}
