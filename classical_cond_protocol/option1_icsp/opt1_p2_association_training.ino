void opt1_p2_association_training() {
    
  // -- Odor association US/CS learning 
  // -- Associative learning parameters TO CHANGE HERE
  // Trial number
  int nb_blocks = 15; // nb of blocks to run
  int nb_trials_per_block=20; // nb of trials per block
  // Durations
  int duration_odor_sampling = 1000; // duration of odor sampling
  int duration_wait = 1000; // waiting duration
  int duration_outcome = 1000; // duration of outcome
  int duration_interstimulus_interval = 8000; // duration of ISI
  int start_assessment_window=1100; // Start of assessment window
  int duration_assessment_window=900; // Duration of assessment window
  // Odors
  int odors[]={2,3}; // odor valves (1 is reserved for blank)
  int odor_valence[]={1,0}; // odor valences (0 = nothing, 1 = water, 2 = air puff)
  String odor_name[]={"EUG","ETHYLACET"}; // odor names
  int nb_odors=2; // nb of odors
  // -- End of parameters

  Serial.print("// ");
  Serial.print(String(millis()));
  Serial.println(",OPTION,1,PHASE,2");
  
  for (int block_id = 1; block_id < (nb_blocks + 1); block_id++) {
    int order[nb_trials_per_block];
    for(int i = 0; i < nb_trials_per_block; i++){
      order[i]=i%2; 
    }
    randomizeArray(order,nb_trials_per_block);
    odor_stimulation(1, block_id, nb_trials_per_block, order, duration_odor_sampling, duration_wait, duration_outcome, duration_interstimulus_interval, start_assessment_window, duration_assessment_window, nb_odors, odors, odor_valence, odor_name);
    checkPauseResume(running_state);
    if(running_state == 2){
       block_id=nb_blocks+1;
    }
  }
  Serial.println("KILL");
}
