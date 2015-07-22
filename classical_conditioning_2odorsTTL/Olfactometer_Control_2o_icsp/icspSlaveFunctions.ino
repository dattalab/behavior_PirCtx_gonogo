 typedef struct olfactoFeedbackStruct
{
  char cmd;
  int param1;
  int param2;
  int param3;
};

void initializeIcspBoard(){
  SPI.begin ();
  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void sendCommandToMaster(struct olfactoFeedbackStruct myCmd){
  digitalWrite(SS, LOW);    // SS is pin 10
  SPI_writeAnything (myCmd);
  digitalWrite(SS, HIGH);
}

void sendFlowInfoToBoard(int param1, int param2, int param3){
  olfactoFeedbackStruct newCmd;
  newCmd.cmd='R';
  newCmd.param1=param1;
  newCmd.param2=param2;
  newCmd.param3=param3;
  sendCommandToMaster(newCmd);
}
