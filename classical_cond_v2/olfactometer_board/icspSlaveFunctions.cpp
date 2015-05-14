/*
 * icspSlaveFunctions.c
 *
 *  Created on: May 13, 2015
 *      Author: ludoviccacheux
 */
#include <Arduino.h>
#include "Olfactometer_header.h"
#include <SPI.h>
#include <SPI_anything.h>
#include "icspSlaveFunctions.h"

 void sendCommandToBoard(struct olfactoFeedbackStruct myCmd){
   digitalWrite(icspOutPin,HIGH);
   SPI_writeAnything(myCmd);
   digitalWrite(icspOutPin,LOW);
 }

 void sendFlowInfoToBoard(int param1, int param2, int param3){
   olfactoFeedbackStruct newCmd;
   newCmd.cmd='R';
   newCmd.param1=param1;
   newCmd.param2=param2;
   newCmd.param3=param3;
   sendCommandToBoard(newCmd);
 }

