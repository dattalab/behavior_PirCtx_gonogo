void randomSequence(int* pRandomSequence, int nb_odors, int nb_trials, int max_consecutive){
  randomSeed(analogRead(randomInPin));
  int nb_generated=nb_odors*nb_trials*2;
  int randomList[nb_generated];
  for(int t=0; t<(nb_generated); t++){
    randomList[t]=random(0,nb_odors);
  }
  int seq_id=0;
  int consec_nb=0;
  int count_trials_per_odor[nb_odors];
  int go=0;
  for(int o=0; o<nb_odors; o++){
    count_trials_per_odor[o]=0;
  }
  pRandomSequence[0]=randomList[0];
  count_trials_per_odor[pRandomSequence[0]]=1;
  for(int i=1; i<(nb_odors*nb_trials); i++){
      if(go == 0){
        seq_id++;
        if(seq_id>nb_generated){
          go=i;
          consec_nb=max_consecutive+1;
        }
        while((go == 0) && ((consec_nb > max_consecutive) || ((count_trials_per_odor[randomList[seq_id]] +1) > nb_trials))){
          seq_id++;
          if(seq_id>nb_generated){
            go=i;
            consec_nb=max_consecutive+1;
          }
          if(pRandomSequence[i-1] != randomList[seq_id]){
            consec_nb=0;
          }
        }
        if(go == 0){
          pRandomSequence[i]=randomList[seq_id];
          count_trials_per_odor[randomList[seq_id]]=count_trials_per_odor[randomList[seq_id]]+1;
          if(pRandomSequence[i-1] == pRandomSequence[i]){
            consec_nb++;
          }
          else{
            consec_nb=0;
          }
        }
      }
  }
  if(go != 0){
    int restingOdors[(nb_odors*nb_trials)-(go-1)];
    int lastIndice=0;
    for(int o=0; o<nb_odors; o++){
      if(count_trials_per_odor[o] < nb_trials){
        for(int i=count_trials_per_odor[o]; i<nb_trials; i++){
          restingOdors[lastIndice]=o;
          lastIndice++;
        }
      }
    }
    for (int i= 0; i< (lastIndice-1); i++) {
      int pos = random(lastIndice-1); 
      int temp = restingOdors[i];  
      restingOdors[i] = restingOdors[pos];
      restingOdors[pos] = temp;
    }
    for(int i=0; i<lastIndice; i++){
      pRandomSequence[go+i]=restingOdors[i];
    }
  }
}
