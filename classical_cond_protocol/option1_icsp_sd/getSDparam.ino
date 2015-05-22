void initializeSD(){
  if (!SD.begin(SD_SELECT))
  while (1)
    Serial.println("// Error: could not initialize SD card interface.");
}
//void printErrorMessage(uint8_t e, bool eol = true)
//{
//  switch (e) {
//  case IniFile::errorNoError:
//    Serial.print("no error");
//    break;
//  case IniFile::errorFileNotFound:
//    Serial.print("file not found");
//    break;
//  case IniFile::errorFileNotOpen:
//    Serial.print("file not open");
//    break;
//  case IniFile::errorBufferTooSmall:
//    Serial.print("buffer too small");
//    break;
//  case IniFile::errorSeekError:
//    Serial.print("seek error");
//    break;
//  case IniFile::errorSectionNotFound:
//    Serial.print("section not found");
//    break;
//  case IniFile::errorKeyNotFound:
//    Serial.print("key not found");
//    break;
//  case IniFile::errorEndOfFile:
//    Serial.print("end of file");
//    break;
//  case IniFile::errorUnknownError:
//    Serial.print("unknown error");
//    break;
//  default:
//    Serial.print("unknown error value");
//    break;
//  }
//  if (eol)
//    Serial.println();
//}

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

void getParamFromSD(char* exp_file, char* step_name){
  getExpParamFromSD(exp_file,step_name);
  //getOdorsetFromSD(odor_set);
  getSettingsFromSD(settings_set);
}

void getExpParamFromSD(char* exp_file, char* step_name){  
  const size_t bufferLen = 80;
  char buffer[bufferLen];
  char exp_filename[40];
  sprintf(exp_filename,"/exp/%s.ini",exp_file);
  Serial.print("// EXPERIMENT CONFIGURATION: ");
  Serial.println(exp_filename);
  IniFile iniExp(exp_filename);
  openIniFile(iniExp);
  
  iniExp.getValue("files", "odor_set", buffer, bufferLen,odor_set,15);
  iniExp.getValue("files", "settings", buffer, bufferLen,settings_set,15);
  
  iniExp.getValue(step_name, "nb_stimuli", buffer, bufferLen,nb_stimuli);
  iniExp.getValue(step_name, "nb_blocks", buffer, bufferLen,nb_blocks);
  iniExp.getValue(step_name, "nb_trials", buffer, bufferLen,nb_trials_per_stim);
  
  iniExp.getValue(step_name, "carrierF_mode", buffer, bufferLen,airflow_modes[0]);
  iniExp.getValue(step_name, "odorF_mode", buffer, bufferLen,airflow_modes[1]);
  
  valves = (int*) malloc(nb_stimuli*sizeof(int));
  valences = (int*) malloc(nb_stimuli*sizeof(int));
  
  if(airflow_modes[0] == 2){
    carrier_rates = (int*) malloc(nb_stimuli*sizeof(int));
  }
  if(airflow_modes[1] == 2){
    odor_rates = (int*) malloc(nb_stimuli*sizeof(int));
  }
  
  for(int s=0; s < nb_stimuli; s++){
    char namefield[20];
    sprintf(namefield,"stimulus%d",s+1);
    //iniExp.getValue(step_name, namefield, buffer, bufferLen,pValves[s]);
    iniExp.getValue(step_name, namefield, buffer, bufferLen,valves[s]);
    sprintf(namefield,"valence%d",s+1);
    iniExp.getValue(step_name, namefield, buffer, bufferLen,valences[s]);
    
    if(airflow_modes[0] == 2){
      sprintf(namefield,"carrierF%d",s+1);
      iniExp.getValue(step_name, namefield, buffer, bufferLen,carrier_rates[s]);
    }
    if(airflow_modes[1] == 2){
      sprintf(namefield,"carrierF%d",s+1);
      iniExp.getValue(step_name, namefield, buffer, bufferLen,odor_rates[s]);
    }
  }
  iniExp.close();
  Serial.println(String(odor_set));
  Serial.println(String(settings_set));
    Serial.println(nb_stimuli);
  for(int i=0; i < nb_stimuli; i++){
    Serial.println(String(valves[i]) + "," + String(valences[i]));
  }
}
//void getOdorsetFromSD(char* odor_set_file){
//  const size_t bufferLen = 80;
//  char buffer[bufferLen];
//  char odor_set_filename[40];
//  sprintf(odor_set_filename,"/osets/%s.ini",odor_set_file);
//  Serial.print("// ODOR SET CONFIGURATION: ");
//  Serial.println(odor_set_filename);
//  IniFile iniOdor(odor_set_filename);
//  openIniFile(iniOdor);
//
//  int nb_odors;
//  iniOdor.getValue("odors","nb_odors",buffer,bufferLen,nb_odors);
//  for(int o=0; o < nb_odors; o++){
//    char namefield[20];
//    sprintf(namefield,"odor%d",o+1);
//    char odor_names[nb_odors][20];
//    iniOdor.getValue("odors",namefield,buffer,bufferLen,odor_names[o],20);
//    Serial.println(odor_names[o]);
//  }
//  iniOdor.close();
//}

void getSettingsFromSD(char* settings_file){
  for(int s=0; s<nb_stimuli; s++){
    //Serial.println(String(odor_names[s]));
  }
  const size_t bufferLen = 80;
  char buffer[bufferLen];
  char settings_filename[40];
  sprintf(settings_filename,"/setts/%s.ini",settings_file);
  Serial.print("// SETTINGS CONFIGURATION: "); 
  Serial.println(settings_filename);
  IniFile iniSett(settings_filename);
  openIniFile(iniSett);
  iniSett.getValue("experiment","exp_mode",buffer,bufferLen,experiment_mode);
  iniSett.getValue("flow","carrier_rate",buffer,bufferLen,flow_rates[0]);
  iniSett.getValue("flow","odor_rate",buffer,bufferLen,flow_rates[1]);
  iniSett.getValue("durations","odor_sampling",buffer,bufferLen,duration_scheme[0]);
  iniSett.getValue("durations","wait",buffer,bufferLen,duration_scheme[1]);
  iniSett.getValue("durations","outcome",buffer,bufferLen,duration_scheme[2]);
  iniSett.getValue("durations","isi",buffer,bufferLen,duration_scheme[3]);
  iniSett.getValue("durations","start_AW",buffer,bufferLen,assessment_window[0]);
  iniSett.getValue("durations","duration_AW",buffer,bufferLen,assessment_window[1]);
  iniSett.close();
  Serial.println(String(experiment_mode));
  Serial.println(String(flow_rates[0]));
  Serial.println(String(flow_rates[1]));
  Serial.println(String(duration_scheme[0]));
  Serial.println(String(duration_scheme[1]));
  Serial.println(String(duration_scheme[2]));
  Serial.println(String(duration_scheme[3]));
  Serial.println(String(assessment_window[0]));
  Serial.println(String(assessment_window[1]));
}
 
void openIniFile(IniFile &ini_file){
  const size_t bufferLen = 80;
  char buffer[bufferLen];
  if (!ini_file.open()) {
    Serial.print("// ERROR: Ini file ");
    Serial.print(ini_file.getFilename());
    Serial.println(" does not exist");
    // Cannot do anything else
    while (1)
      ;
  }
  // Check the file is valid. This can be used to warn if any lines
  // are longer than the buffer.
//  if (!ini_file.validate(buffer, bufferLen)) {
//    Serial.print("// Error: ini file ");
//    Serial.print(ini_file.getFilename());
//    Serial.print(" not valid: ");
//    printErrorMessage(ini_file.getError());
//    // Cannot do anything else
//    while (1)
//      ;
//  }
}
