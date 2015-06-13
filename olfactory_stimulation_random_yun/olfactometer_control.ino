#include <YunClient.h>

byte server[] = {192, 168, 20, 85};
int port = 3336;

void initializeOlfacto() {
  byte connexion = 0;
  while (connexion != 1) {
    connexion = client.connect(server, port);
    if (connexion == 1) {
      writeOut((String) F("// Connected!"));
    }
    else {
      writeOut((String) F("// Connection failed!"));
      delay(500);
    }
  }
}

// This is the function used to send a command to the olfactometer
void sendCommandToOlfacto(String myCmd) {
  for (int i = 0; i < (myCmd.length() + 1); i++) {
    client.write(myCmd[i]);
  }
  client.write('\n');
  client.flush();
}

void idleOlfacto() {
  for (int i = 0; i < carriers_nb; i++) {
    updateFlowOlfacto(0, carriers[i], default_carrierRate);
  }
  for (int i = 0; i < bankflows_nb; i++) {
    updateFlowOlfacto(1, bankflows[i], default_odorRate);
    blankBank(i);
  }
}

void openValve(int valve) {
  if (valve < 16) {
    sendCommandToOlfacto((String) "write Bank" + bankflows[0] + F("_Valves ") + valve);
  }
  else if (valve < 24) {
    sendCommandToOlfacto((String) "write Bank" + bankflows[1] + F("_Valves ") + valve);
  }
}

void closeValve(int valve) {
  if (valve < 16) {
    blankBank(bankflows[0]);
  }
  else if (valve < 24) {
    blankBank(bankflows[1]);
  }
}

void blankBank(int bank) {
  sendCommandToOlfacto((String) F("write Bank") + bank + F("_Valves 0"));
}

void updateFlowOlfacto(int type, int id, int flow) {
  switch (type) {
    case 0: // carrier
      sendCommandToOlfacto((String) F("write Carrier") + id + F("_Actuator ") + flow);
      break;
    case 1:
      sendCommandToOlfacto((String) F("write BankFlow") + id + F("_Actuator ") + flow);
      break;
  }
}
