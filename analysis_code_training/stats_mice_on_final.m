clear all

FileName=0;
while(FileName == 0)
    [FileName,PathName] = uigetfile('*.mat',strcat('Select final data file'));
end

load(fullfile(PathName,FileName));

stat_conc_allm_correct_score=do_stats_mice(conc_allm_correct_score);
stat_conc_allm_correct_score_odors=do_stats_mice(conc_allm_correct_score_odors);
stat_conc_allm_correct_score_sep=do_stats_mice(conc_allm_correct_score_sep);
stat_conc_allm_mean_licking=do_stats_mice(conc_allm_mean_licking);
stat_conc_allm_mean_licking_latency=do_stats_mice(conc_allm_mean_licking_latency);
stat_conc_allm_mean_licking_odors=do_stats_mice(conc_allm_mean_licking_odors);
stat_conc_allm_mean_licking_latency_odors=do_stats_mice(conc_allm_mean_licking_latency_odors);
stat_conc_allm_mean_licking_odors_sep=do_stats_mice(conc_allm_mean_licking_odors_sep);
stat_conc_allm_mean_licking_latency_odors_sep=do_stats_mice(conc_allm_mean_licking_latency_odors_sep);

save(fullfile(PathName,strrep(FileName,'.mat','_stats.mat')),'stat_*','conc_allm_nb_*','conc_allm_odors','list_datafiles');

clear all