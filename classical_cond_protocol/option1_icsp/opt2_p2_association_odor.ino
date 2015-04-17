void opt2_p2_association_odor() {
  
// -- Odor association US/CS learning 
// -- Associative learning parameters TO CHANGE HERE
// Trial number
int nb_blocks = 3; // nb of blocks to run
int nb_trials_per_block=100; // nb of trials per block
// Durations
int duration_odor_sampling = 1000; // duration of odor sampling
int duration_wait = 1000; // waiting duration
int duration_outcome = 1000; // duration of outcome
int duration_interstimulus_interval = 1000; // duration of ISI
int start_assessment_window=1000; // Start of assessment window
int duration_assessment_window=8000; // Duration of assessment window
// Odors
int odors[]={2,3}; // odor valves (1 is reserved for blank)
int odor_valence[]={1,0}; // odor valences (0 = nothing, 1 = water, 2 = air puff)
String odor_name[]={"EUG","ETHYLACET"}; // odor names
int nb_odors=2; // nb of odors
// -- End of parameters

    Serial.print("// ");
    Serial.print(String(millis()));
    Serial.println(",OPTION,2,PHASE,2");
    
  for (int current_block = 1; current_block < (nb_blocks + 1); current_block++) {
    int order[nb_trials_per_block];
    for(int i = 0; i < nb_trials_per_block; i++){
      order[i]=i%2; 
    }
    odor_stimulation(1, current_block,  order, duration_odor_sampling, duration_wait, duration_outcome, duration_interstimulus_interval, start_assessment_window, duration_assessment_window, odors, odor_valence, odor_name);
  }
  Serial.println("KILL");
}
