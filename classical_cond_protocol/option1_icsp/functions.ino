void deliverWaterReward(int duration_water_reward){
  digitalWrite(solenoidOutPin,HIGH);
  delay(duration_water_reward);
  digitalWrite(solenoidOutPin,LOW);
}

int lengthArrayByte(byte array[]){
  return (sizeof(array)/sizeof(int));
}

int lengthArrayInt(const int* array, size_t size){
  int mySize;
  mySize=sizeof(array)/sizeof(int);
  return mySize;
}
