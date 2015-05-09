
#include "Arduino.h"

// =================================
// Pin Assignments
int RTS_pin = A5;
int BNC_pin = 2;
int V1 = 3;
int V2 = 4;
int V3 = 5;
int V4 = 6;
int V5 = 7;
int V6 = 8;
int V7 = 9;
int V8 = 10;
int V9 = 11;
int V10 = 12;
int V11 = 13;
int V12 = A0;
int V13 = A1;
int V14 = A2;
int V15 = A3;
int V16 = A4;

int num_valves = 16;
int all_valves[] = {V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12, V13, V14, V15, V16};

// =======================================
//     MFC Addresses
// (DO NOT CHANGE THESE)
char* default_address = "11";
char* carrier_address = "02";
char* odor_address = "04";

