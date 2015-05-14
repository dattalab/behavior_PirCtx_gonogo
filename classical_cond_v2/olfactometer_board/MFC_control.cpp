/*
 * MFC_control.cpp
 *
 *  Created on: May 13, 2015
 *      Author: ludoviccacheux
 */



// =======================================
// MFC control functions

#include <Arduino.h>
#include "Olfactometer_header.h"
#include "MFC_control.h"

boolean MFC_DEBUG=false;
int command_delay_time=10;
char MFCBuffer[1024];

void MFC_Println(char* text) {
  digitalWrite(RTS_pin, HIGH); // RTS low
  Serial1.println(text);
  Serial1.flush();
  digitalWrite(RTS_pin, LOW); // RTS low

  if (MFC_DEBUG) {
    Serial.print(F("==> "));
    Serial.println(text);
  }
}

void MFC_Read() {
  char inByte;
  int len;
  MFCBuffer[0] = 0;
  len = Serial1.readBytesUntil('\n', MFCBuffer, 1024);
  MFCBuffer[len] = 0;
  if (MFC_DEBUG) {
    Serial.print(F("<--   "));
    Serial.println(MFCBuffer);
  }
}


void setMFCToDigital(char* address) {
   String command = "!";
   command += address;
   command += ",M,D";
   command.toCharArray(MFCBuffer,1024);
   MFC_Println(MFCBuffer);
   MFC_Read();
   String response = MFCBuffer;

   if (!response.startsWith(String("!"+ String(address)))) {
     Serial.println(F("ERROR: bad MFC reponse."));
     return;
   }
}


float setMFCFlow(char* address, float flowRate) {
   String command = "!";
   command += address;
   command += ",S,";
   command += flowRate;
   command.toCharArray(MFCBuffer,1024);
   MFC_Println(MFCBuffer);
   MFC_Read();
   String response = MFCBuffer;

   if (!response.startsWith(String("!"+ String(address)+",S"))) {
     Serial.println(F("ERROR: bad MFC reponse."));
     return -1;
   }

   float val = response.substring(4).toFloat();

   return val;
}

float getMFCFlowRate(char* address) {
   String command = "!";
   command += address;
   command += ",F";
   command.toCharArray(MFCBuffer,1024);
   MFC_Println(MFCBuffer);
   MFC_Read();
   String response = MFCBuffer;

   if (!response.startsWith(String("!"+ String(address)))) {
     Serial.println(F("ERROR: bad MFC response."));
     return -1.0;
   }

   float val = response.substring(4).toFloat();

   return val;
}

void getMFCModelNumber(char* address) {
   String command = "!";
   command += address;
   command += ",MR,2";
   command.toCharArray(MFCBuffer,1024);
   MFC_Println(MFCBuffer);
   MFC_Read();
}


void setMFCUnitsToSLPM(char* address) {
   String command = "!";
   command += address;
   command += ",U,SLPM";
   command.toCharArray(MFCBuffer,1024);
   MFC_Println(MFCBuffer);
   MFC_Read();
   String response = MFCBuffer;

   if (!response.startsWith(String("!"+ String(address)+ ",USLPM"))) {
     Serial.println(F("ERROR: bad MFC response to units change."));
     return;
   }
}


void setupMFC(char* address, float flowRate) {
  getMFCModelNumber(address);
  delay(command_delay_time);
  setMFCToDigital(address);
  delay(command_delay_time);
  setMFCUnitsToSLPM(address);
  delay(command_delay_time);
  setMFCFlow(address, flowRate);
  delay(command_delay_time);
}

void changeMFCFlowRate(char* address, float flowRate) {
  setMFCFlow(address, flowRate);
  delay(command_delay_time);
}



//====================================
//    Initial MFC Setup
//====================================

// BE CAREFUL when changing the address of an MFC! If you forget the new address -- or
// accidentally change to an incorrect address -- you will be unable to communicate with
// the MFC at all. (You will then be forced to search through the full address space
// until you can communicate with it again.)

// You should only change the address of an MFC once. All Omega FMA6500 MFCs come from
// the factory with a default address of "11" ('default_address'). Depending on the way
// the MFC will be used, we change the address to either 'carrier_address' = "02" or
// 'odor_address' = "04".

// Only use changeMCFAddress_Odor() and changeMCFAddress_Carrier() to perform address changes.



void setMFCAddress(char* current_address, char* new_address); // do not call this directly
void changeMFCAddress_core(char* new_address, char* new_address_name); // do not call this directly

void changeMFCAddress_Odor() {changeMFCAddress_core(odor_address,"ODOR");}
void changeMFCAddress_Carrier() {changeMFCAddress_core(carrier_address,"CARRIER");}



// DO NOT RUN THIS COMMAND DIRECTLY. Use the two commands below instead.
void setMFCAddress(char* current_address, char* new_address) {
   String command = "!";
   command += current_address;
   command += ",MW,7,";
   command += new_address;
   command.toCharArray(MFCBuffer,1024);
   MFC_Println(MFCBuffer);
   MFC_Read();
   String response = MFCBuffer;

   if (!response.startsWith(String("!"+ String(new_address)))) {
     Serial.println(F("ERROR: bad MFC response."));
     return;
   }
}

// do not call this directly
void changeMFCAddress_core(char* new_address, char* new_address_name) {

  Serial.println(F("WARNING:"));
  Serial.println(F("You are about to change the address of an MFC."));
  Serial.println(F(""));
  Serial.println(F("Please make sure that:"));
  Serial.println(F(" 1) The Arduino is connected to *only one* MFC."));
  Serial.println(F(" 2) The MFC should still have the default factory address."));
  Serial.println(F(""));
  Serial.println(F("If these conditions are met, enter 'Y':"));

  // wait for 'Y'
  while (1) {
    if (Serial.available() > 0) {
      // read the incoming byte:
      char incomingByte = Serial.read();
      if ((incomingByte == 'Y') || (incomingByte == 'y')) {
        break;
      }
    }
  }

  Serial.println(F("Step 1: Establishing communications with default address..."));
  getMFCModelNumber(default_address);
  Serial.print(F("Requesting Model Number: "));
  Serial.println(MFCBuffer);
  delay(command_delay_time);

  float flow = getMFCFlowRate(default_address);
  Serial.print(F("Requesting Flow Rate: "));
  Serial.println(flow);

  if (flow < 0.0) {
    Serial.println(F("ERROR: Bad flow rate (<0), aborting."));
    return;
  }

  Serial.println(F("Please confirm that: "));
  Serial.println(F(" 1) Model number and flow rate values seem correct"));
  Serial.print(F(" 2) You plan to use this MFC as an "));
  Serial.print(new_address_name);
  Serial.println(F(" MFC"));
  Serial.println(F(""));
  Serial.println(F("Enter 'Y' to confirm and proceed with address change."));

  // wait for 'Y'
  while (1) {
    if (Serial.available() > 0) {
      // read the incoming byte:
      char incomingByte = Serial.read();
      if ((incomingByte == 'Y') || (incomingByte == 'y')) {
        break;
      }
    }
  }

  Serial.println(F("Step 2: Attempting address change..."));
  setMFCAddress(default_address, new_address);

  Serial.println(F("Step 3: Establishing communications with new address..."));
  getMFCModelNumber(new_address);
  Serial.print(F("Requesting Model Number: "));
  Serial.println(MFCBuffer);
  delay(command_delay_time);

  flow = getMFCFlowRate(new_address);
  Serial.print(F("Requesting Flow Rate: "));
  Serial.println(flow);

  if (flow < 0.0) {
    Serial.println(F("ERROR: Bad flow rate (<0), aborting."));
    return;
  } else {
    Serial.println(F("Address change successful!"));
  }
}

