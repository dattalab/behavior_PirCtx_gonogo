clear all

previous_config=0;

if(previous_config ~= 1)
    disp('# File selection for concatenation across mice')
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
max_nb_sessions=NaN(1,nb_datafiles);
for i=1:nb_datafiles
    fprintf('Processing %s...\n',fullfile(list_datafiles{i,2},list_datafiles{i,1}));
    load(fullfile(list_datafiles{i,2},list_datafiles{i,1}),'all_max_nb_blocks','all_max_nb_odors','all_max_nb_trials','all_nb_sessions');
    max_nb_blocks(i)=all_max_nb_blocks;
    max_nb_trials(i)=all_max_nb_trials;
    max_nb_odors(i)=all_max_nb_odors;
    max_nb_sessions(i)=all_nb_sessions;
    clear all_max_nb_blocks all_max_nb_trials all_max_nb_odors all_max_nb_sessions
end

disp('# Files concatenation')
[~, deepestFolder, ~] = fileparts(pwd);
FileName=0;
while(isequal(FileName,0))
    [FileName,PathName] = uiputfile('*.mat','Save dataset as',fullfile(pwd,strcat(deepestFolder,'.mat')));
end
allm_max_nb_blocks=max(max_nb_blocks);
allm_max_nb_trials=max(max_nb_trials);
allm_max_nb_odors=max(max_nb_odors);
allm_max_nb_sessions=max(max_nb_sessions);
allm_nb_mice=nb_datafiles;

allm_scorer=NaN(allm_max_nb_trials,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_iti=NaN(allm_max_nb_trials,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_odor_identity=NaN(allm_max_nb_trials,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_licking_latency=NaN(allm_max_nb_trials,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_duration_licks=NaN(allm_max_nb_trials,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_correct_score=NaN(allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_correct_score_sep=NaN(2,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_correct_score_odors=NaN(allm_max_nb_odors,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_mean_licking=NaN(2,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_mean_licking_odors=NaN(allm_max_nb_odors,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_mean_licking_latency=NaN(2,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_mean_licking_latency_odors=NaN(allm_max_nb_odors,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_median_licking=NaN(2,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_median_licking_odors=NaN(allm_max_nb_odors,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_mean_licking_odors_sep=NaN(2,allm_max_nb_odors,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_mean_nb_licking=NaN(2,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_mean_nb_licking_odors=NaN(allm_max_nb_odors,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_mean_nb_licking_odors_sep=NaN(2,allm_max_nb_odors,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_nb_trials_per_block=NaN(allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_nb_blocks=NaN(allm_max_nb_sessions,allm_nb_mice);
allm_nb_licks=NaN(allm_max_nb_trials,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_odor_identity=NaN(allm_max_nb_trials,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_odors={};
allm_resume_score=NaN(4,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);
allm_resume_score_odors=NaN(4,allm_max_nb_odors,allm_max_nb_blocks,allm_max_nb_sessions,allm_nb_mice);

for f=1:nb_datafiles
    fprintf('Opening %s...\n',fullfile(list_datafiles{f,2},list_datafiles{f,1}));
    load(fullfile(list_datafiles{f,2},list_datafiles{f,1}),'all_*');
    allm_nb_sessions(f)=all_nb_sessions;
    for s=1:allm_nb_sessions(f)
        allm_nb_blocks(s,f)=all_nb_blocks(s);
        all_nb_odors=size(all_odors{1,s}.valence,1);

        for b=1:allm_nb_blocks(s,f)
            allm_nb_trials_per_block(b,s,f)=all_nb_trials_per_block(b,s);

            allm_scorer(1:allm_nb_trials_per_block(b,s,f),b,s,f)=all_scorer(1:allm_nb_trials_per_block(b,s,f),b,s);
            allm_iti(1:allm_nb_trials_per_block(b,s,f),b,s,f)=all_iti(1:allm_nb_trials_per_block(b,s,f),b,s);
            allm_odor_identity(1:allm_nb_trials_per_block(b,s,f),b,s,f)=all_odor_identity(1:allm_nb_trials_per_block(b,s,f),b,s);
            allm_licking_latency(1:allm_nb_trials_per_block(b,s,f),b,s,f)=all_licking_latency(1:allm_nb_trials_per_block(b,s,f),b,s);
            allm_duration_licks(1:allm_nb_trials_per_block(b,s,f),b,s,f)=all_duration_licks(1:allm_nb_trials_per_block(b,s,f),b,s);
            allm_correct_score(b,s,f)=all_correct_score(b,s);
            allm_correct_score_sep(:,b,s,f)=all_correct_score_sep(:,b,s);
            allm_correct_score_odors(1:all_nb_odors,b,s,f)=all_correct_score_odors(1:all_nb_odors,b,s);

            allm_mean_licking(:,b,s,f)=all_mean_licking(:,b,s);
            allm_mean_licking_odors(1:all_nb_odors,b,s,f)=all_mean_licking_odors(1:all_nb_odors,b,s);
            allm_mean_licking_odors_sep(:,1:all_nb_odors,b,s,f)=all_mean_licking_odors_sep(:,1:all_nb_odors,b,s);
            allm_median_licking(:,b,s,f)=all_median_licking(:,b,s);
            allm_median_licking_odors(1:all_nb_odors,b,s,f)=all_median_licking_odors(1:all_nb_odors,b,s);


            allm_mean_licking_latency(:,b,s,f)=all_mean_licking_latency(:,b,s);
            allm_mean_licking_latency_odors(1:all_nb_odors,b,s,f)=all_mean_licking_latency_odors(1:all_nb_odors,b,s);
            allm_mean_licking_latency_odors_sep(:,1:all_nb_odors,b,s,f)=all_mean_licking_latency_odors_sep(:,1:all_nb_odors,b,s);
            allm_median_licking_latency(:,b,s,f)=all_median_licking_latency(:,b,s);
            allm_median_licking_latency_odors_sep(:,b,s,f)=all_median_licking_latency_odors_sep(:,b,s);
            allm_median_licking_latency_odors(1:all_nb_odors,b,s,f)=all_median_licking_latency_odors(1:all_nb_odors,b,s);

            allm_mean_nb_licking(:,b,s,f)=all_mean_nb_licking(:,b,s);
            allm_mean_nb_licking_odors(1:all_nb_odors,b,s,f)=all_mean_nb_licking_odors(1:all_nb_odors,b,s);
            allm_mean_nb_licking_odors_sep(:,1:all_nb_odors,b,s,f)=allm_mean_nb_licking_odors_sep(:,1:all_nb_odors,b,s);

            allm_nb_licks(1:allm_nb_trials_per_block(b,s,f),b,s,f)=all_nb_licks(1:allm_nb_trials_per_block(b,s,f),b,s);
            allm_odor_identity(1:allm_nb_trials_per_block(b,s,f),b,s,f)=all_odor_identity(1:allm_nb_trials_per_block(b,s,f),b,s);
            allm_resume_score(:,b,s,f)=all_resume_score(:,b,s);
            allm_resume_score_odors(:,1:all_nb_odors,b,s,f)=all_resume_score_odors(:,1:all_nb_odors,b,s);
        end
        allm_odors{s,f}=struct;
        allm_odors{s,f}.name=all_odors{1,s}.name;
        allm_odors{s,f}.valence=all_odors{1,s}.valence;
        allm_odors{s,f}.valve=all_odors{1,s}.valve;
    end
    clear all_*
%     clear all_* correct_score correct_score_odors duration_licks iti licking_latency mean_licking mean_licking_odors median_licking median_licking_odors mean_licking_latency mean_licking_latency_odors mean_nb_licking mean_nb_licking_odors nb_blocks nb_licks nb_trials_per_block odor_identity odors resume_score resume_score_odors scorer
end

save(fullfile(PathName,FileName),'allm_*','list_datafiles');
clear all