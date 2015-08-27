// -- Wiring information
const byte buttonInPin = 2; // Push button
const byte LED1OutPin = 6; // Green LED
const byte LED2OutPin = 7; // Red LED
const byte icspOutPin = 8; // ICSP Communication Pin
const byte lickInPin = 3; // Capacitor
const byte solenoidOutPin = 4;
const byte punishmentOutPin = 5; // Airpuff
int randomInPin = A11; // Analog input for random seed generation

// --

void initializePinStatus(){
  // set pin status
  pinMode(solenoidOutPin,OUTPUT);
  digitalWrite(solenoidOutPin,LOW);
  pinMode(randomInPin,INPUT);
  pinMode(buttonInPin,INPUT);
  pinMode(LED1OutPin,OUTPUT);
  pinMode(LED2OutPin,OUTPUT);
  pinMode(icspOutPin,OUTPUT);
  pinMode(lickInPin,INPUT);
  
  pinMode(punishmentOutPin,OUTPUT);
  
  digitalWrite(LED1OutPin,LOW);
  digitalWrite(LED2OutPin,LOW);
  
  digitalWrite(punishmentOutPin,LOW);
  digitalWrite(icspOutPin,LOW);
}
