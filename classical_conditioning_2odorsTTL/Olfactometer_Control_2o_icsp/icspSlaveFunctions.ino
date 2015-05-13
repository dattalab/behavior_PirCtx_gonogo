 typedef struct olfactoFeedbackStruct
{
  char cmd;
  int param1;
  int param2;
  int param3;
};

void sendCommandToMaster(struct olfactoFeedbackStruct myCmd){
  digitalWrite(icspOutPin,HIGH);
  SPI_writeAnything(myCmd);
  digitalWrite(icspOutPin,LOW);
}

void sendFlowInfoToMaster(int param1, int param2, int param3){
  olfactoFeedbackStruct newCmd;
  newCmd.cmd='R';
  newCmd.param1=param1;
  newCmd.param2=param2;
  newCmd.param3=param3;
  sendCommandToMaster(newCmd);
}
