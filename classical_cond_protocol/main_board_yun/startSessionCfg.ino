//#include <ArduinoJson.h>
// #include <Bridge.h>
void startSessionCfg(String id_config, String id_step) {
  String odor_wheel[] = {F("EUGENOL"), F("ETHYLACETATE"), F("ALPHA-PINENE"), F("THIAZOLIDINE"), F("ALPHA-IONONE"), F("BUTYRICACID"), F("2-AMINOACETOPHENONE")};
  // -- Odor association US/CS learning
  // -- Associative learning parameters TO CHANGE HERE
  int settings[3];
  settings[0] = 1;
  settings[1] = 15; // nb of blocks to run
  settings[2] = 10; // nb of trials of each odor
  int default_flows[2];
  default_flows[0] = 1000;
  default_flows[1] = 100;
  // Durations
  int duration_scheme[6];
  duration_scheme[0] = 1000; // duration of odor sampling
  duration_scheme[1] = 1000; // waiting duration
  duration_scheme[2] = 1000; // duration of outcome
  duration_scheme[3] = 0; // duration of ISI
  duration_scheme[4] = 0; // Start of assessment window
  duration_scheme[5] = 1999; // Duration of assessment window
  // Odors
  int valves[] = {2, 3}; // odor valves
  int valence[] = {1,0}; // odor valences (0 = nothing, 1 = water, 2 = air puff)
  int nb_stimuli = 2; // nb of odors
  String odor_name[nb_stimuli];
  int carrier_flows[nb_stimuli];
  int odor_flows[nb_stimuli];
  // -- End of parameters

  for (int a = 0; a < nb_stimuli; a++) {
    odor_name[a]=odor_wheel[valves[a]-2];
    carrier_flows[a] = default_flows[0];
    odor_flows[a] = default_flows[1];
  }
  int nb_blocks=settings[1];
  int nb_trials_per_block=settings[2];
  Serial.println((String) millis() + F(",CONOK,1"));
  Serial.println((String) F("// ") + millis() + F(",OPTION,1,PHASE,2"));

  for (int block_id = 1; block_id < (nb_blocks + 1); block_id++) {
    int order[nb_stimuli*nb_trials_per_block];
    int i=0;
    for (int o = 0; o < nb_stimuli; o++) {
      for(int t=0; t<nb_trials_per_block; t++){
        order[i] = o;
        i++;
      }
    }
    randomizeArray(order, nb_stimuli*nb_trials_per_block);
    odor_stimulation(settings[0], block_id, nb_stimuli * nb_trials_per_block, order, duration_scheme[0], duration_scheme[1], duration_scheme[2], duration_scheme[3], duration_scheme[4], duration_scheme[5], nb_stimuli, valves, valence, odor_name, default_flows, carrier_flows, odor_flows);
    checkPauseResume();
    if (*pRunningState == 2) {
      block_id = nb_blocks + 1;
    }
  }
  Serial.println(F("KILL"));
}
//
////{
//
//int catch_stimuli = 8;
//
//String odor_name[10];
//int carrier_flows[10];
//int valves[10];
//int odor_flows[10];
//int valences[10];
//int default_flows[2];
//int duration_scheme[6];
//int settings[3];
//int nb_stimuli;
//
//// -- Odor association US/CS learning
//Bridge.put("config_acquired", "0");
//p.begin("/mnt/sda1/arduino/www/catch_param_uno2.py");
//p.addParameter(id_config);
//p.addParameter(id_step);
//p.run();
//delay(20000);
////sendCommandToOlfacto((String) F("GET_CONF,1,1"));
//// Parameter acquisition
//char duration_scheme_b[10];
//Bridge.get("duration_scheme(0)", duration_scheme_b, 10);
//Serial.println(duration_scheme_b);
//Serial.println("done");
////char buffer3[buffer2.length()];
////buffer2.toCharArray(buffer3,buffer2.length()+1);
////Serial.println(buffer2);
////Serial.println(buffer3);
////      DynamicJsonBuffer jsonBuffer;
////      JsonObject& root = jsonBuffer.parseObject(buffer3);
////      if (!root.success()) {
////        Serial.println("// Unable to parse configuration.");
////        return;
////      }
////      Mailbox.end();
////      for(int a= 0; a < 4; a++){
////        settings[a]=root["settings"][a].as<int>();
////      }
////      for(int a= 0; a < 7; a++){
////        duration_scheme[a]=root["duration_scheme"][a].as<int>();
////      }
////      default_flows[0]=root["default_flows"][0].as<int>();
////      default_flows[1]=root["default_flows"][1].as<int>();
////      nb_stimuli=root["nb_stimuli"].as<int>();
//for (int a = 0; a < 2; a++) {
//  //        odor_name[a]=root["stimulus_set"][a][0].asString();
//  //        carrier_flows[a]=root["stimulus_set"][a][1].as<int>();
//  //        valves[a]=root["stimulus_set"][a][2].as<int>();
//  //        odor_flows[a]=root["stimulus_set"][a][3].as<int>();
//  //        valences[a]=root["stimulus_set"][a][4].as<int>();
//  if (carrier_flows[a] == 0) {
//    carrier_flows[a] = default_flows[0];
//  }
//  if (odor_flows[a] == 0) {
//    odor_flows[a] = default_flows[1];
//  }
//}
////int nb_blocks=settings[1];
////int nb_trials_per_block=settings[2];
//
//Serial.print("// " + String(millis()) + ",OPTION,1,PHASE,2");
//
//for (int block_id = 1; block_id < (nb_blocks + 1); block_id++) {
//  int order[nb_stimuli * nb_trials_per_block];
//  for (int i = 0; i < nb_stimuli * nb_trials_per_block; i++) {
//    order[i] = i % nb_stimuli;
//  }
//  randomizeArray(order, nb_trials_per_block);
//  odor_stimulation(settings[0], block_id, nb_stimuli * nb_trials_per_block, order, duration_scheme[0], duration_scheme[1], duration_scheme[2], duration_scheme[3], duration_scheme[4], duration_scheme[5], nb_stimuli, valves, valences, odor_name, default_flows, carrier_flows, odor_flows);
//  checkPauseResume();
//  if (*pRunningState == 2) {
//    block_id = nb_blocks + 1;
//  }
//}
//Serial.println(F("KILL"));
//}
