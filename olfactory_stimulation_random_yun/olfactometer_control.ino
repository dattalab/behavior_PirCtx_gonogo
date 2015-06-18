#include <YunClient.h>

byte server[] = {192, 168, 20, 85};
int port = 3336;

void initializeOlfacto() {
  boolean connexion = false;
  while (connexion != true) {
    connexion = client.connect(server, port);
    if (connexion == true) {
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
  client.println(myCmd);
  client.flush();
}

void idleOlfacto() {
  updateFlowOlfacto(0, default_carrierRate);
  updateFlowOlfacto(1, default_odorRate);
  for (int i = 0; i < bankflows_nb; i++) {
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

void setFlowOlfacto(int type, int id, int flow) {
  switch (type) {
    case 0: // carrier
      sendCommandToOlfacto((String) F("write Carrier") + id + F("_Actuator ") + flow);
      break;
    case 1:
      sendCommandToOlfacto((String) F("write BankFlow") + id + F("_Actuator ") + flow);
      break;
  }
}

void updateFlowOlfacto(int type, int flow) {
  switch (type) {
    case 0: // carrier
      for (int i = 0; i < carriers_nb; i++) {
        setFlowOlfacto(0, carriers[i], (int) floor(flow / 2));
      }
      break;
    case 1:
      for (int i = 0; i < bankflows_nb; i++) {
        setFlowOlfacto(1, bankflows[i], (int) floor(flow / 2));
      }
      break;
  }
}
