/*
 * MFC_control.h
 *
 *  Created on: May 13, 2015
 *      Author: ludoviccacheux
 */

#ifndef MFC_CONTROL_H_
#define MFC_CONTROL_H_

void MFC_Println(char* text);

void MFC_Read();


void setMFCToDigital(char* address);
float setMFCFlow(char* address, float flowRate);
float getMFCFlowRate(char* address);

void getMFCModelNumber(char* address);

void setMFCUnitsToSLPM(char* address);


void setupMFC(char* address, float flowRate);

void changeMFCFlowRate(char* address, float flowRate);

void changeMFCAddress_Odor();
void changeMFCAddress_Carrier();

#endif /* MFC_CONTROL_H_ */
