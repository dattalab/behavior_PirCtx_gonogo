#include <Bridge.h>

void startSessionCfg(String id_config, String id_step) {
  // put your main code here, to run repeatedly:
  Console.println((String) F("// Loading configuration: ") + id_config + " " + id_step);
  get_conf.begin(F("/mnt/sda1/arduino/www/catch_param_uno2.py"));
  get_conf.addParameter(id_config);
  get_conf.addParameter(id_step);
  get_conf.run();
  int nb_stimuli=getIntBridge("nb_stimuli");
  int settings[3];
  int default_flows[2];
  int duration_scheme[6];
  String odor_name[nb_stimuli];
  int carrier_flows[nb_stimuli];
  int odor_flows[nb_stimuli];
  int valves[nb_stimuli];
  int valences[nb_stimuli];
  for (int a = 0; a < 3; a++) {
    settings[a] = getIntBridge(fstring("basic_settings(%i)", a));
  }
  for (int a = 0; a < 6; a++) {
    duration_scheme[a] = getIntBridge(fstring("duration_scheme(%i)", a));
  }
  for (int a = 0; a < 2; a++) {
    default_flows[a] = getIntBridge(fstring("default_flows(%i)", a));
  }
  for (int a = 0; a < nb_stimuli; a++) {
    odor_name[a] = getStringBridge(fstring("s(%i)(0)", a));
    carrier_flows[a] = getIntBridge(fstring("s(%i)(1)", a));
    valves[a] = getIntBridge(fstring("s(%i)(2)", a));
    odor_flows[a] = getIntBridge(fstring("s(%i)(3)", a));
    valences[a] = getIntBridge(fstring("s(%i)(4)", a));
    if (carrier_flows[a] == 0) {
      carrier_flows[a] = default_flows[0];
    }
    if (odor_flows[a] == 0) {
      odor_flows[a] = default_flows[1];
    }
  }
  int nb_blocks = settings[1];
  int nb_trials_per_block = settings[2];
  Console.println((String) millis() + F(",CONOK,1"));
  Console.println((String) F("// ") + millis() + F(",OPTION,1,PHASE,2"));

  for (int block_id = 1; block_id < (nb_blocks + 1); block_id++) {
    int order[nb_stimuli * nb_trials_per_block];
    int i = 0;
    for (int o = 0; o < nb_stimuli; o++) {
      for (int t = 0; t < nb_trials_per_block; t++) {
        order[i] = o;
        i++;
      }
    }
    randomizeArray(order, nb_stimuli * nb_trials_per_block);
    odor_stimulation(settings[0], block_id, nb_stimuli * nb_trials_per_block, order, duration_scheme[0], duration_scheme[1], duration_scheme[2], duration_scheme[3], duration_scheme[4], duration_scheme[5], nb_stimuli, valves, valences, odor_name, default_flows, carrier_flows, odor_flows);
    checkPauseResume();
    if (*pRunningState == 2) {
      block_id = nb_blocks + 1;
    }
  }
  Console.println(F("KILL"));
}
