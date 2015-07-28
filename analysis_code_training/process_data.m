clear all

previous_config=0;

if(previous_config ~= 1)
    disp('# File selection')
    FileName='A';
    nb_datafiles=0;
    list_datafiles={};
    while(FileName ~= 0)
        [FileName,PathName] = uigetfile('*.mat',strcat('Select data file #',num2str(nb_datafiles+1),' or cancel to end selection'));
        if(FileName ~= 0)
            nb_datafiles=nb_datafiles+1;
            list_datafiles{nb_datafiles,1}=FileName;
            list_datafiles{nb_datafiles,2}=PathName;
        end
    end
else
    FileName=0;
    while(FileName == 0)
        [FileName,PathName] = uigetfile('*.mat','Select original file');
    end
    load(fullfile(PathName,FileName),'list_datafiles');
    nb_datafiles=size(list_datafiles,1);
end

disp('# Files processing')
max_nb_blocks=NaN(1,nb_datafiles);
max_nb_trials=NaN(1,nb_datafiles);
max_nb_odors=NaN(1,nb_datafiles);
for i=1:nb_datafiles
    fprintf('Processing %s...\n',fullfile(list_datafiles{i,2},list_datafiles{i,1}));
    [max_nb_blocks(i),max_nb_trials(i),max_nb_odors(i)]=do_score(list_datafiles{i,2},list_datafiles{i,1},0);
end

disp('# Files concatenation')
[~, deepestFolder, ~] = fileparts(pwd);
FileName=0;
while(isequal(FileName,0))
    [FileName,PathName] = uiputfile('*.mat','Save dataset as',fullfile(pwd,strcat(deepestFolder,'.mat')));
end
all_max_nb_blocks=max(max_nb_blocks);
all_max_nb_trials=max(max_nb_trials);
all_max_nb_odors=max(max_nb_odors);
all_nb_sessions=nb_datafiles;

all_scorer=NaN(all_max_nb_trials,all_max_nb_blocks,all_nb_sessions);
all_iti=NaN(all_max_nb_trials,all_max_nb_blocks,all_nb_sessions);
all_odor_identity=NaN(all_max_nb_trials,all_max_nb_blocks,all_nb_sessions);
all_licking_latency=NaN(all_max_nb_trials,all_max_nb_blocks,all_nb_sessions);
all_duration_licks=NaN(all_max_nb_trials,all_max_nb_blocks,all_nb_sessions);
all_correct_score=NaN(all_max_nb_blocks,all_nb_sessions);
all_correct_score_sep=NaN(2,all_max_nb_blocks,all_nb_sessions);
all_correct_score_odors=NaN(all_max_nb_odors,all_max_nb_blocks,all_nb_sessions);
all_mean_licking=NaN(2,all_max_nb_blocks,all_nb_sessions);
all_mean_licking_odors=NaN(all_max_nb_odors,all_max_nb_blocks,all_nb_sessions);
all_mean_licking_latency=NaN(2,all_max_nb_blocks,all_nb_sessions);
all_mean_licking_latency_odors=NaN(all_max_nb_odors,all_max_nb_blocks,all_nb_sessions);
all_median_licking=NaN(2,all_max_nb_blocks,all_nb_sessions);
all_median_licking_odors=NaN(all_max_nb_odors,all_max_nb_blocks,all_nb_sessions);
all_mean_licking_odors_sep=NaN(2,all_max_nb_odors,all_max_nb_blocks,all_nb_sessions);
all_mean_nb_licking=NaN(2,all_max_nb_blocks,all_nb_sessions);
all_mean_nb_licking_odors=NaN(all_max_nb_odors,all_max_nb_blocks,all_nb_sessions);
all_mean_nb_licking_odors_sep=NaN(2,all_max_nb_odors,all_max_nb_blocks,all_nb_sessions);
all_nb_trials_per_block=NaN(all_max_nb_blocks,all_nb_sessions);
all_nb_blocks=[];
all_nb_licks=NaN(all_max_nb_trials,all_max_nb_blocks,all_nb_sessions);
all_odor_identity=NaN(all_max_nb_trials,all_max_nb_blocks,all_nb_sessions);
all_odors={};
all_resume_score=NaN(4,all_max_nb_blocks,all_nb_sessions);
all_resume_score_odors=NaN(4,all_max_nb_odors,all_max_nb_blocks,all_nb_sessions);

for f=1:nb_datafiles
    fprintf('Opening %s...\n',fullfile(list_datafiles{f,2},strrep(list_datafiles{f,1},'.mat','_results.mat')));
    load(fullfile(list_datafiles{f,2},strrep(list_datafiles{f,1},'.mat','_results.mat')));
    all_nb_blocks(f)=nb_blocks;
    
    for b=1:nb_blocks
        all_nb_trials_per_block(b,f)=nb_trials_per_block;
        
        all_scorer(1:nb_trials_per_block,b,f)=scorer(:,b);
        all_iti(1:nb_trials_per_block,b,f)=iti(:,b);
        all_odor_identity(1:nb_trials_per_block,b,f)=odor_identity(:,b);
        all_licking_latency(1:nb_trials_per_block,b,f)=licking_latency(:,b);
        all_duration_licks(1:nb_trials_per_block,b,f)=duration_licks(:,b);
        all_correct_score(b,f)=correct_score(b);
        all_correct_score_sep(:,b,f)=correct_score_sep(:,b);
        all_correct_score_odors(1:nb_odors,b,f)=correct_score_odors(:,b);
        
        all_mean_licking(:,b,f)=mean_licking(:,b);
        all_mean_licking_odors(1:nb_odors,b,f)=mean_licking_odors(:,b);
        all_mean_licking_odors_sep(:,1:nb_odors,b,f)=mean_licking_odors_sep(:,:,b);
        all_median_licking(:,b,f)=median_licking(:,b);
        all_median_licking_odors(1:nb_odors,b,f)=median_licking_odors(:,b);
        
        
        all_mean_licking_latency(:,b,f)=mean_licking_latency(:,b);
        all_mean_licking_latency_odors(1:nb_odors,b,f)=mean_licking_latency_odors(:,b);
        all_mean_licking_latency_odors_sep(:,1:nb_odors,b,f)=mean_licking_latency_odors_sep(:,:,b);
        all_median_licking_latency(:,b,f)=median_licking_latency(:,b);
        all_median_licking_latency_odors_sep(:,b,f)=median_licking_latency_odors_sep(:,b);
        all_median_licking_latency_odors(1:nb_odors,b,f)=median_licking_latency_odors(:,b);
        
        all_mean_nb_licking(:,b,f)=mean_nb_licking(:,b);
        all_mean_nb_licking_odors(1:nb_odors,b,f)=mean_nb_licking_odors(:,b);
        all_mean_nb_licking_odors_sep(:,1:nb_odors,b,f)=mean_nb_licking_odors_sep(:,:,b);
        
        all_nb_licks(1:nb_trials_per_block,b,f)=nb_licks(:,b);
        all_odor_identity(1:nb_trials_per_block,b,f)=odor_identity(:,b);
        all_resume_score(:,b,f)=resume_score(:,b);
        all_resume_score_odors(:,1:nb_odors,b,f)=resume_score_odors(:,:,b);
    end
    all_odors{f}=struct;
    all_odors{f}.name=odors.name;
    all_odors{f}.valence=odors.valence;
    all_odors{f}.valve=odors.valve;
    clear correct_score correct_score_odors duration_licks iti licking_latency mean_licking mean_licking_odors median_licking median_licking_odors mean_licking_latency mean_licking_latency_odors mean_nb_licking mean_nb_licking_odors nb_blocks nb_licks nb_trials_per_block odor_identity odors resume_score resume_score_odors scorer
end

save(fullfile(PathName,FileName),'all_*','list_datafiles');
clear all