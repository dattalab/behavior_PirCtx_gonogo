// Definition of the command structure used for ICSP communication with the olfactometer (this has to match the one of the olfactometer; otherwise communication will not be possible)
// There are 2 fields (command / parameter) 
typedef struct commandStruct
{
  char cmd;
  int param;
};

// This is the function used to send a command to the olfactometer
// The icspOutPin serves to signal communication to the olfactometer and is switched to HIGH before transmission / switched back to LOW after transmission
void sendCommandToOlfacto(struct commandStruct myCmd){
  digitalWrite(icspOutPin,HIGH);
  SPI_writeAnything(myCmd);
  digitalWrite(icspOutPin,LOW);
}

// To idle the olfactometer, we send "I/1"
void idleOlfacto(){
  commandStruct newCmd;
  newCmd.cmd='I';
  newCmd.param=1;
  sendCommandToOlfacto(newCmd);
}

// To start a new trial, we send "T/X" where X is the number of the valve
void stimulusOlfacto(int v){
  commandStruct newCmd;
  newCmd.cmd='T';
  newCmd.param=v;
  sendCommandToOlfacto(newCmd);
}

// To set trial duration, we send "D/X" where X is the duration of the stimulus (ms)
void setDurationOlfacto (int duration){
  commandStruct newCmd;
  newCmd.cmd='D';
  newCmd.param=duration;
  sendCommandToOlfacto(newCmd);
}
