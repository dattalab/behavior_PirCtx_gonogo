/*
 * Olfactometer_header.h
 *
 *  Created on: May 14, 2015
 *      Author: ludoviccacheux
 */

// User-modifiable global variables:
// =================================
#ifndef OLFACTOMETER_HEADER_H_
#define OLFACTOMETER_HEADER_H_

extern bool ONE_VALVE_OPEN; // When true, exactly one valve is open at a time. Value 1 is open by default.
                               // When false, every value openeing and closing is explicitly controlled by user.

extern bool DISPLAY_VALVE_STATUS; // Write all valve openings/closing to USB

extern float CARRIER_FLOW_RATE; // default carrier rate in LPM
extern float ODOR_FLOW_RATE; // default carrier rate in LPM

// =================================
extern int stimulus_duration; // default stimulus duration

extern int last_odor;

// =======================================
// Valve control functions

extern int current_valve;
extern float lastCarrierFlowMeasure;
extern float lastOdorFlowMeasure;

// ==============

// Pin Assignments
extern int RTS_pin;
extern int icspOutPin;
extern int V1;
extern int V2;
extern int V3;
extern int V4;
extern int V5;
extern int V6;
extern int V7;
extern int V8;
extern int V9;
extern int V10;
extern int V11;
extern int V12;
extern int V13;
extern int V14;
extern int V15;
extern int V16;

extern int num_valves;
extern int all_valves[];

// =======================================
//     MFC Addresses
extern char* default_address;
extern char* carrier_address;
extern char* odor_address;

struct commandStruct
{
  char cmd='Z';
  int param=0;
};

#endif /* OLFACTOMETER_HEADER_H_ */
