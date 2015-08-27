clear all

FileName=0;
while(FileName == 0)
    [FileName,PathName] = uigetfile('*.mat',strcat('Select concatenated data file'));
end

load(fullfile(PathName,FileName));

conc_allm_correct_score=NaN(allm_max_nb_blocks*allm_max_nb_sessions,allm_nb_mice);
conc_allm_correct_score_odors=NaN(allm_max_nb_odors,allm_max_nb_blocks*allm_max_nb_sessions,allm_nb_mice);
conc_allm_correct_score_sep=NaN(2,allm_max_nb_blocks*allm_max_nb_sessions,allm_nb_mice);

conc_allm_mean_licking=NaN(2,allm_max_nb_blocks*allm_max_nb_sessions,allm_nb_mice);
conc_allm_mean_licking_latency=NaN(2,allm_max_nb_blocks*allm_max_nb_sessions,allm_nb_mice);
conc_allm_mean_licking_odors=NaN(allm_max_nb_odors,allm_max_nb_blocks*allm_max_nb_sessions,allm_nb_mice);
conc_allm_mean_licking_latency_odors=NaN(allm_max_nb_odors,allm_max_nb_blocks*allm_max_nb_sessions,allm_nb_mice);
conc_allm_mean_licking_odors_sep=NaN(2,allm_max_nb_odors,allm_max_nb_blocks*allm_max_nb_sessions,allm_nb_mice);
conc_allm_mean_licking_latency_odors_sep=NaN(2,allm_max_nb_odors,allm_max_nb_blocks*allm_max_nb_sessions,allm_nb_mice);

for m=1:allm_nb_mice
    for s=1:allm_nb_sessions(m)
        for b=1:allm_nb_blocks(s,m)
            if((sum(isnan(allm_odor_identity(:,b,s,m))) + sum(allm_odor_identity(:,b,s,m)) == 0) == 0)
                absolute_block=sum(~isnan(conc_allm_correct_score(:,m)))+1;
                conc_allm_correct_score(absolute_block,m)=allm_correct_score(b,s,m);
                conc_allm_correct_score_odors(1:size(allm_odors{s,m}.valence,1),absolute_block,m)=allm_correct_score_odors(:,b,s,m);
                conc_allm_correct_score_sep(:,absolute_block,m)=allm_correct_score_sep(:,b,s,m);
                conc_allm_mean_licking(:,absolute_block,m)=allm_mean_licking(:,b,s,m);
                conc_allm_mean_licking_latency(:,absolute_block,m)=allm_mean_licking_latency(:,b,s,m);
                conc_allm_mean_licking_odors(1:size(allm_odors{s,m}.valence,1),absolute_block,m)=allm_mean_licking_odors(:,b,s,m);
                conc_allm_mean_licking_latency_odors(1:size(allm_odors{s,m}.valence,1),absolute_block,m)=allm_mean_licking_latency_odors(:,b,s,m);
                conc_allm_mean_licking_odors_sep(:,1:size(allm_odors{s,m}.valence,1),absolute_block,m)=allm_mean_licking_odors_sep(:,:,b,s,m);
                conc_allm_mean_licking_latency_odors_sep(:,1:size(allm_odors{s,m}.valence,1),absolute_block,m)=allm_mean_licking_latency_odors_sep(:,:,b,s,m);
            end
        end
    end
end

conc_allm_nb_true_blocks=sum(~isnan(conc_allm_correct_score));
conc_allm_nb_max_true_blocks=max(conc_allm_nb_true_blocks);
conc_allm_nb_min_true_blocks=min(conc_allm_nb_true_blocks);
conc_allm_correct_score(conc_allm_nb_max_true_blocks+1:end,:)=[];
conc_allm_correct_score_odors(:,conc_allm_nb_max_true_blocks+1:end,:)=[];
conc_allm_correct_score_sep(:,conc_allm_nb_max_true_blocks+1:end,:)=[];
conc_allm_mean_licking(:,conc_allm_nb_max_true_blocks+1:end,:)=[];
conc_allm_mean_licking_latency(:,conc_allm_nb_max_true_blocks+1:end,:)=[];
conc_allm_mean_licking_odors(:,conc_allm_nb_max_true_blocks+1:end,:)=[];
conc_allm_mean_licking_latency_odors(:,conc_allm_nb_max_true_blocks+1:end,:)=[];
conc_allm_mean_licking_odors_sep(:,:,conc_allm_nb_max_true_blocks+1:end,:)=[];
conc_allm_mean_licking_latency_odors_sep(:,:,conc_allm_nb_max_true_blocks+1:end,:)=[];

conc_allm_max_nb_blocks=allm_max_nb_blocks;
conc_allm_max_nb_odors=allm_max_nb_odors;
conc_allm_max_nb_sessions=allm_max_nb_sessions;
conc_allm_max_nb_trials=allm_max_nb_trials;
conc_allm_nb_blocks=allm_nb_blocks;
conc_allm_nb_mice=allm_nb_mice;
conc_allm_nb_sessions=allm_nb_sessions;
conc_allm_nb_trials_per_block=allm_nb_trials_per_block;
conc_allm_odors=allm_odors;

save(fullfile(PathName,strrep(FileName,'.mat','_conc.mat')),'conc_*','list_datafiles');

clear all