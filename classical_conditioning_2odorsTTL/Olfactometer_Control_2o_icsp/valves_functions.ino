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
