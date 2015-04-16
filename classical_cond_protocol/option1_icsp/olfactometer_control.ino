typedef struct commandStruct
{
  char cmd;
  int param;
};

void sendCommandToOlfacto(struct commandStruct myCmd){
  digitalWrite(icspOutPin,HIGH);
  SPI_writeAnything(myCmd);
  digitalWrite(icspOutPin,LOW);
}

void idleOlfacto(){
  commandStruct newCmd;
  newCmd.cmd='I';
  newCmd.param=1;
  sendCommandToOlfacto(newCmd);
}

void stimulusOlfacto(int v){
  commandStruct newCmd;
  newCmd.cmd='T';
  newCmd.param=v;
  sendCommandToOlfacto(newCmd);
}

void setDurationOlfacto (int duration){
  commandStruct newCmd;
  newCmd.cmd='D';
  newCmd.param=duration;
  sendCommandToOlfacto(newCmd);
}
