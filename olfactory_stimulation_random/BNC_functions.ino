void sendTriggerTTL(){
  digitalWrite(trigger, HIGH); 
  delay(pulse_length);
  digitalWrite(trigger, LOW);
}
