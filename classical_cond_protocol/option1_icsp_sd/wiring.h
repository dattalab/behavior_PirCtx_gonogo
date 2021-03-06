// -- Wiring information
const byte buttonInPin = 2; // Push button
const byte LED1OutPin = 6; // Green LED
const byte LED2OutPin = 7; // Red LED
const byte icspInPin = 8; // ICSP Communication Pin
const byte lickInPin = 3; // Capacitor
const byte solenoidOutPin = 4;
const byte punishmentOutPin = 5; // Airpuff
int randomInPin = A0; // Analog input for random seed generation

// --

void initializePinStatus(){
  // set pin status
  pinMode(randomInPin,INPUT);
  pinMode(buttonInPin,INPUT);
  pinMode(LED1OutPin,OUTPUT);
  pinMode(LED2OutPin,OUTPUT);
  pinMode(icspInPin,INPUT);
  pinMode(lickInPin,INPUT);
  pinMode(solenoidOutPin,OUTPUT);
  pinMode(punishmentOutPin,OUTPUT);
  
  digitalWrite(LED1OutPin,LOW);
  digitalWrite(LED2OutPin,LOW);
  digitalWrite(solenoidOutPin,LOW);
  digitalWrite(punishmentOutPin,LOW);
}
