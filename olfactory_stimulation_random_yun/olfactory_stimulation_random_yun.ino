#include <Bridge.h>
#include <YunClient.h>

// Pin assignments
int trigger = 7;
int TTL = 6;
int randomInPin = 8;

// TTL parameters
int pulse_length = 100;

// These variables are used to know what program to launch & when
int current_phase = 1; // program to launch
byte current_go = 0; // set to 0 to wait, 1 to start immediately
int running_state = 1; // this is a little indicator to be able to stop/resume/pause the execution
int* pRunningState;
char serial_read;
int lastRequestedFlows[2];

int carriers[] = {1, 2};
int carriers_nb = 2;
int bankflows[] = {2, 3};
int bankflows_nb = 2;
int default_odorRate = 136;
int default_carrierRate = 1000;

YunClient client;
Process get_conf;

void setup() {
  Bridge.begin();
  Serial.begin(115200);
  initializeOlfacto();
  idleOlfacto();

  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW);
  pinMode(TTL, OUTPUT);
  digitalWrite(TTL, LOW);
  pinMode(randomInPin, INPUT);
}

String last_command = "";
String id_config = "";
String id_step = "";

void processCommand(String command) {
  int new_flow;
  int id_valve;
  char cmd = getValue(command, ',', 0)[0];
  switch (cmd) {
    case 'M':
      new_flow = getValue(command, ',', 1).toInt();
      updateFlowOlfacto(0, new_flow);
      writeOut((String) "// Carrier flow updated to " + new_flow);
      break;
    case 'F':
      new_flow = getValue(command, ',', 1).toInt();
      updateFlowOlfacto(1, new_flow);
      writeOut((String) "// Odor flow updated to " + new_flow);
      break;
    case 'P':
      id_config = getValue(command, ',', 1);
      id_step = getValue(command, ',', 2);
      break;
    case 'T':
      id_valve = getValue(command, ',', 1).toInt();
      openValve(id_valve);
      delay(2000);
      closeValve(id_valve);
      break;
    case 'X':
      writeOut(F("// Loading configuration in the Bridge..."));
      get_conf.begin(F("/mnt/sda1/arduino/www/catch_param_scope.py"));
      get_conf.addParameter(id_config);
      get_conf.addParameter(id_step);
      get_conf.run();
      writeOut(F("// Configuration loaded in the Bridge."));
      break;
    case 'W':
      updateFlowOlfacto(0, 2000);
      updateFlowOlfacto(1, 2000);
      writeOut(F("// Washing odor streams ..."));
      while (true) {
        for (int valve = 1; valve < 23; valve++) {
          writeOut((String) F("// Valve ") + valve);
          openValve(valve);
          delay(5000);
          closeValve(valve);
        }
      }
      break;
    case 'V':
      writeOut(F("// Loading odor lines (5s pulses) ..."));
      for (int valve = 1; valve < 23; valve++) {
        writeOut((String) F("// Valve ") + valve);
        openValve(valve);
        delay(5000);
        closeValve(valve);
      }
      writeOut(F("// Done loading odor lines"));
      break;
    case 'Z':
      current_go = 1;
      break;
  }
}

void loop() {
  // This is the main entrance, it actually waits for Serial input to know what program to start and when
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      processCommand(last_command);
      last_command = "";
    }
    else if (c != '\r') {
      last_command += c;
    }
  }

  // if a start command has been received or if the push button has been pressed, start the program
  if (current_go == 1) {
    current_go = 0;
    running_state = 1;
    switch (current_phase) {
      case 1:
        if (id_config.length() != 0) {
          startSessionCfg(id_config, id_step);
          break;
        }
        else {
          writeOut(F("// FATAL ERROR: Undefined configuration."));
        }
    }
  }
}
