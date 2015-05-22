void experimentFromSD(char* exp_file, char* step_name){
  
    char odor_set[15];
    char settings_set[15];
    int nb_stimuli;
    int nb_blocks;
    int nb_trials_per_stim;
    int* valves;
    int* valences;
    int* carrier_rates;
    int* odor_rates;
    int experiment_mode;
    int flow_rates[2];
    int duration_scheme[4];
    int assessment_window[2];
    int airflow_modes[2];
    
    getParamFromSD("exp1","step1");
    
    const size_t bufferLen = 80;
    char buffer[bufferLen];
    char odor_set_filename[40];
    sprintf(odor_set_filename,"/osets/%s.ini",odor_set);
    Serial.print("// ODOR SET CONFIGURATION: ");
    Serial.println(odor_set_filename);
    IniFile iniOdor(odor_set_filename);
    openIniFile(iniOdor);
  
    int nb_odors;
    char odor_names[nb_stimuli][20];
    iniOdor.getValue("odors","nb_odors",buffer,bufferLen,nb_odors);
    for(int s=0; s < nb_stimuli; s++){
      char namefield[20];
      sprintf(namefield,"odor%d",valves[s]);
      iniOdor.getValue("odors",namefield,buffer,bufferLen,odor_names[s],20);
      Serial.print(odor_names[s]);
    }
    iniOdor.close();
    
    for (int block_id = 1; block_id < (nb_blocks + 1); block_id++) {
      int order[nb_trials_per_stim*nb_stimuli];
      for(int s=0; s < nb_stimuli; s++){
        for(int i = 0; i < nb_trials_per_stim*nb_stimuli; i++){
          order[s*nb_trials_per_stim+i]=s;
        }
      }
      randomizeArray(order,nb_trials_per_stim*nb_stimuli);
      odor_stimulation(experiment_mode, block_id, nb_trials_per_stim*nb_stimuli, order, duration_scheme[0], duration_scheme[1], duration_scheme[2], duration_scheme[3], assessment_window[0], assessment_window[1], nb_odors, valves, valences, odor_names);
      checkPauseResume();
      if(*pRunningState == 2){
          block_id=nb_blocks+1;
      }
    }
    Serial.println("KILL");
}
