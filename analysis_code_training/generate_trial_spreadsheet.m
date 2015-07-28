clear all

FileName=0;
while(FileName == 0)
    [FileName,PathName] = uigetfile('*.mat',strcat('Select concatenated data file'));
end

mouse_id_prompt=inputdlg('Enter mouse ID');
mouse_id=str2num(mouse_id_prompt{1,1});

load(fullfile(PathName,FileName));

MOUSE_ID=[];
ABSOLUTE_BLOCK=[];
SESSIONS=[];
BLOCKS=[];
TRIALS=[];
ODORS=[];
LICKING_LATENCY=[];
ITI=[];
LICKING_DURATION=[];
SCORER=[];
REWARD_CONDITION=[];

i=0;
for s=1:length(all_nb_blocks)
    for b=1:all_nb_blocks(s)
        i=i+1;
        for t=1:all_nb_trials_per_block(b,s)
            if(~isnan(all_odor_identity(t,b,s)))
                MOUSE_ID=[MOUSE_ID;mouse_id];
                ABSOLUTE_BLOCK=[ABSOLUTE_BLOCK;i];
                SESSIONS=[SESSIONS;s];
                BLOCKS=[BLOCKS;b];
                TRIALS=[TRIALS;t];
                ODORS=[ODORS;all_odor_identity(t,b,s)];
                LICKING_LATENCY=[LICKING_LATENCY;all_licking_latency(t,b,s)];
                ITI=[ITI;all_iti(t,b,s)];
                LICKING_DURATION=[LICKING_DURATION;all_duration_licks(t,b,s)];
                SCORER=[SCORER;all_scorer(t,b,s)];
                REWARD_CONDITION=[REWARD_CONDITION;all_odors{1,s}.valence(all_odor_identity(t,b,s),b)];
            end
        end
    end
end

T=[MOUSE_ID ABSOLUTE_BLOCK SESSIONS,BLOCKS,TRIALS,ODORS,SCORER,LICKING_LATENCY,LICKING_DURATION,ITI,REWARD_CONDITION];
csvwrite(strrep(fullfile(PathName,FileName),'.mat','_trials.csv'),T);

clear T MOUSE_ID ABSOLUTE_BLOCK SESSIONS BLOCKS TRIALS ODORS LICKING_LATENCY ITI LICKING_DURATION SCORER REWARD_CONDITION

MOUSE_ID=[];
ABSOLUTE_BLOCK=[];
SESSIONS=[];
BLOCKS=[];
ODORS=[];
REWARD_CONDITION=[];
SCORE=[];
MEAN_LICKING_DURATION=[];
MEDIAN_LICKING_DURATION=[];
MEAN_LICKING_LATENCY=[];
MEDIAN_LICKING_LATENCY=[];

i=0;
for s=1:length(all_nb_blocks)
    for b=1:all_nb_blocks(s)
        i=i+1;
        for o=1:length(all_odors{1,s}.valence(:,b))
            if(~isnan(all_odors{1,s}.valence(o,b)))
                MOUSE_ID=[MOUSE_ID;mouse_id];
                ABSOLUTE_BLOCK=[ABSOLUTE_BLOCK;i];
                SESSIONS=[SESSIONS;s];
                BLOCKS=[BLOCKS;b];
                ODORS=[ODORS;o];
                REWARD_CONDITION=[REWARD_CONDITION;all_odors{1,s}.valence(o,b)];
                MEAN_LICKING_DURATION=[MEAN_LICKING_DURATION;all_mean_licking_odors(o,b,s)];
                MEDIAN_LICKING_DURATION=[MEDIAN_LICKING_DURATION;all_median_licking_odors(o,b,s)];
                MEAN_LICKING_LATENCY=[MEAN_LICKING_LATENCY;all_mean_licking_latency_odors(o,b,s)];
                MEDIAN_LICKING_LATENCY=[MEDIAN_LICKING_LATENCY;all_median_licking_latency_odors(o,b,s)];
                SCORE=[SCORE;all_correct_score_odors(o,b,s)];
            end
        end
    end
end

T=[MOUSE_ID,ABSOLUTE_BLOCK,SESSIONS,BLOCKS,ODORS,REWARD_CONDITION,SCORE,MEAN_LICKING_DURATION,MEDIAN_LICKING_DURATION,MEAN_LICKING_LATENCY,MEDIAN_LICKING_LATENCY];
csvwrite(strrep(fullfile(PathName,FileName),'.mat','_performanceperodor.csv'),T);

clear T MOUSE_ID ABSOLUTE_BLOCK SESSIONS BLOCKS ODORS REWARD_CONDITION SCORE MEAN_LICKING_DURATION MEDIAN_LICKING_DURATION MEAN_LICKING_LATENCY MEDIAN_LICKING_LATENCY

MOUSE_ID=[];
ABSOLUTE_BLOCK=[];
SESSIONS=[];
BLOCKS=[];
SCORE_TOTAL=[];
SCORE_REWARDED=[];
SCORE_UNREWARDED=[];
MEAN_LICKING_REWARDED=[];
MEDIAN_LICKING_REWARDED=[];
MEAN_LICKING_UNREWARDED=[];
MEDIAN_LICKING_UNREWARDED=[];
MEAN_LICKING_LATENCY_REWARDED=[];
MEDIAN_LICKING_LATENCY_REWARDED=[];
MEAN_LICKING_LATENCY_UNREWARDED=[];
MEDIAN_LICKING_LATENCY_UNREWARDED=[];

i=0;
for s=1:length(all_nb_blocks)
    for b=1:all_nb_blocks(s)
        i=i+1;
        MOUSE_ID=[MOUSE_ID;mouse_id];
        ABSOLUTE_BLOCK=[ABSOLUTE_BLOCK;i];
        SESSIONS=[SESSIONS;s];
        BLOCKS=[BLOCKS;b];
        SCORE_TOTAL=[SCORE_TOTAL;all_correct_score(b,s)];
        SCORE_REWARDED=[SCORE_REWARDED;all_correct_score_sep(1,b,s)];
        SCORE_UNREWARDED=[SCORE_UNREWARDED;all_correct_score_sep(2,b,s)];
        MEAN_LICKING_REWARDED=[MEAN_LICKING_REWARDED;all_mean_licking(1,b,s)];
        MEDIAN_LICKING_REWARDED=[MEDIAN_LICKING_REWARDED;all_median_licking(1,b,s)];
        MEAN_LICKING_UNREWARDED=[MEAN_LICKING_UNREWARDED;all_mean_licking(2,b,s)];
        MEDIAN_LICKING_UNREWARDED=[MEDIAN_LICKING_UNREWARDED;all_median_licking(2,b,s)];
        MEAN_LICKING_LATENCY_REWARDED=[MEAN_LICKING_LATENCY_REWARDED;all_mean_licking_latency(1,b,s)];
        MEDIAN_LICKING_LATENCY_REWARDED=[MEDIAN_LICKING_LATENCY_REWARDED;all_median_licking_latency(1,b,s)];
        MEAN_LICKING_LATENCY_UNREWARDED=[MEAN_LICKING_LATENCY_UNREWARDED;all_mean_licking_latency(2,b,s)];
        MEDIAN_LICKING_LATENCY_UNREWARDED=[MEDIAN_LICKING_LATENCY_UNREWARDED;all_median_licking_latency(2,b,s)];
        
    end
end

T=[MOUSE_ID,ABSOLUTE_BLOCK,SESSIONS,BLOCKS,SCORE_TOTAL,SCORE_REWARDED,SCORE_UNREWARDED,MEAN_LICKING_REWARDED,MEDIAN_LICKING_REWARDED,MEAN_LICKING_UNREWARDED,MEDIAN_LICKING_UNREWARDED,MEAN_LICKING_LATENCY_REWARDED,MEDIAN_LICKING_LATENCY_REWARDED,MEAN_LICKING_LATENCY_UNREWARDED,MEDIAN_LICKING_LATENCY_UNREWARDED];
csvwrite(strrep(fullfile(PathName,FileName),'.mat','_performanceperblock.csv'),T);

clear MOUSE_ID ABSOLUTE_BLOCK SESSIONS BlOCKS SCORE_TOTAL SCORE_REWARDED SCORE_UNREWARDED MEAN_LICKING_REWARDED MEDIAN_LICKING_REWARDED MEAN_LICKING_UNREWARDED MEDIAN_LICKING_UNREWARDED MEAN_LICKING_LATENCY_REWARDED MEDIAN_LICKING_LATENCY_REWARDED MEAN_LICKING_LATENCY_UNREWARDED MEDIAN_LICKING_LATENCY_UNREWARDED

T=cell(length(all_odors{1,1}.valence(:,1)),4);
for o=1:length(all_odors{1,1}.valence(:,1))
    T(o,:)=[{mouse_id},o,{all_odors{1,1}.name{o,1}},{all_odors{1,1}.valence(o,1)}];
end
cell2csv(strrep(fullfile(PathName,FileName),'.mat','_odorlist.csv'),T);
clear T

clear all