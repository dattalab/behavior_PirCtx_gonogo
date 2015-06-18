#include <Bridge.h>

void startSessionCfg(String id_config, String id_step) {
  // put your main code here, to run repeatedly:
  writeOut((String) F("// Loading configuration: ") + id_config + " " + id_step);
  get_conf.begin(F("/mnt/sda1/arduino/www/catch_param_uno2.py"));
  get_conf.addParameter(id_config);
  get_conf.addParameter(id_step);
  get_conf.run();
  int nb_stimuli=getIntBridge("nb_stimuli");
  int settings[3];
  int default_flows[2];
  int duration_scheme[7];
  String odor_name[nb_stimuli];
  int carrier_flows[nb_stimuli];
  int odor_flows[nb_stimuli];
  int valves[nb_stimuli];
  int valences[nb_stimuli];
  int nb_trials_per_odor[nb_stimuli];
  int nb_trials_per_block=0;
  for (int a = 0; a < 3; a++) {
    settings[a] = getIntBridge(fstringF(F("basic_settings/%i"), a));
  }
  for (int a = 0; a < 6; a++) {
    duration_scheme[a] = getIntBridge(fstringF(F("duration_scheme/%i"), a));
  }
  if(settings[0] == 2){
    duration_scheme[6]=getIntBridge("duration_scheme/6");
  }
  else{
    duration_scheme[6]=0;
  }
  for (int a = 0; a < 2; a++) {
    default_flows[a] = getIntBridge(fstringF(F("default_flows/%i"), a));
  }
  for (int a = 0; a < nb_stimuli; a++) {
    odor_name[a] = getStringBridge(fstringF(F("s/%i/0"), a));
    carrier_flows[a] = getIntBridge(fstringF(F("s/%i/1"), a));
    valves[a] = getIntBridge(fstringF(F("s/%i/2"), a));
    odor_flows[a] = getIntBridge(fstringF(F("s/%i/3"), a));
    valences[a] = getIntBridge(fstringF(F("s/%i/4"), a));
    nb_trials_per_odor[a]=getIntBridge(fstring(F("s/%i/5"), a));
    if (carrier_flows[a] == 0) {
      carrier_flows[a] = default_flows[0];
    }
    if (odor_flows[a] == 0) {
      odor_flows[a] = default_flows[1];
    }
    if(nb_trials_per_odor[a] == 0){
      nb_trials_per_odor[a]=settings[2];
    }
    nb_trials_per_block=nb_trials_per_block+nb_trials_per_odor[a];
  }
  int nb_blocks = settings[1];
  writeOut(fstringF(F("%lu,CONOK,1"),millis()));
  writeOut(fstringF(F("//%lu,OPTION,1,PHASE,2"),millis()));

  for (int block_id = 1; block_id < (nb_blocks + 1); block_id++) {
    int order[nb_trials_per_block];
    int i = 0;
//    for(int t=0; t < nb_trials_per_block; t++){
//      order[t]=t%nb_stimuli;
//    }
    for (int o = 0; o < nb_stimuli; o++) {
      for (int t = 0; t < nb_trials_per_odor[o]; t++) {
        order[i] = o;
        i++;
      }
    }
    randomizeArray(order, nb_trials_per_block);
    odor_stimulation(settings[0], block_id, nb_trials_per_block, order, duration_scheme[0], duration_scheme[1], duration_scheme[2], duration_scheme[3], duration_scheme[4], duration_scheme[5], duration_scheme[6], nb_stimuli, valves, valences, odor_name, default_flows, carrier_flows, odor_flows);
    checkPauseResume();
    if (*pRunningState == 2) {
      block_id = nb_blocks + 1;
    }
  }
  writeOut(F("KILL"));
}
