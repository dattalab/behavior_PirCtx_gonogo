void deliverWaterReward(int duration_water_reward){
  digitalWrite(solenoidOutPin,HIGH);
  delay(duration_water_reward);
  digitalWrite(solenoidOutPin,LOW);
}

void randomizeArray(int *parray, int nb_elements){
  for (int i= 0; i< nb_elements; i++) {
    int pos = random(nb_elements-1); 
    int temp = parray[i];  
    parray[i] = parray[pos];
    parray[pos] = temp;
  }
}
