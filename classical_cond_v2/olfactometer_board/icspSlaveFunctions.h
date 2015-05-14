/*
 * icspSlaveFunctions.h
 *
 *  Created on: May 13, 2015
 *      Author: ludoviccacheux
 */

#ifndef ICSPSLAVEFUNCTIONS_H_
#define ICSPSLAVEFUNCTIONS_H_

struct olfactoFeedbackStruct
{
  char cmd;
  int param1;
  int param2;
  int param3;
};
 void sendCommandToBoard(struct olfactoFeedbackStruct myCmd);
 void sendFlowInfoToBoard(int param1, int param2, int param3);

#endif /* ICSPSLAVEFUNCTIONS_H_ */
