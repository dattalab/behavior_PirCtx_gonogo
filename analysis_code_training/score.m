nb_blocks=length(block_param.nb_trials);
%nb_blocks=3;
nb_trials_per_block=block_param.nb_trials(1);
%nb_trials_per_block=19;
nb_odors=size(odors.valence,1);

if(size(lick_events,1) < nb_trials_per_block)
    for i=size(lick_events,1):nb_trials_per_block
        lick_events{i,1}=[];
    end
end

scorer=NaN(nb_trials_per_block,nb_blocks);
nb_licks=NaN(nb_trials_per_block,nb_blocks);

for b=1:nb_blocks
    if(sum(trial_info.odor_identity(:,b) == 0) == 0)
        for t=1:nb_trials_per_block
            if(odors.valence(trial_info.odor_identity(t,b),b) == 1)
                if(size(lick_events{t,b},1) > 0)
                    if(length(find((lick_events{t,b}(:,1) < 2000))) > 0)
                        scorer(t,b)=1;
                    else
                        scorer(t,b)=2;
                    end
                    nb_licks(t,b)=length(find((lick_events{t,b}(:,1) < 2000)));
                else
                    scorer(t,b)=2;
                    nb_licks(t,b)=0;
                end
            elseif(odors.valence(trial_info.odor_identity(t,b),b) == 0)
                if(size(lick_events{t,b},1) > 0)
                    if(length(find((lick_events{t,b}(:,1) < 2000))) > 0)
                        scorer(t,b)=3;
                    else
                        scorer(t,b)=4;
                    end
                    nb_licks(t,b)=length(find((lick_events{t,b}(:,1) < 2000)));
                else
                    scorer(t,b)=4;
                    nb_licks(t,b)=0;
                end
            end
        end
    end
end

duration_licks=NaN(nb_trials_per_block,nb_blocks);
licking_latency=NaN(nb_trials_per_block,nb_blocks);
for b=1:nb_blocks
    if(sum(trial_info.odor_identity(:,b) == 0) == 0)
        for t=1:nb_trials_per_block
            duration_licks(t,b)=0;
            if(size(lick_events{t,b},1) > 0)
                row=[];
                [row, col]=find((lick_events{t,b}(:,1) < 2000));
                licking_latency(t,b)=lick_events{t,b}(1,1);
                if(length(row) > 0)
                    for i=1:length(row)
                        if((lick_events{t,b}(row(i),2) < 2000) && (lick_events{t,b}(row(i),2) > 0))
                            end_lick=lick_events{t,b}(row(i),2);
                        else
                            end_lick=2000;
                        end
                        duration_licks(t,b)=duration_licks(t,b)+(end_lick-lick_events{t,b}(row(i),1));
                    end
                end
            end
        end
    end
end

save('_results.mat','nb_blocks','nb_trials_per_block','scorer','nb_licks','duration_licks','licking_latency');

% correct_score=NaN(1,nb_blocks);
% correct_score_odors=NaN(nb_odors,nb_blocks);
% resume_score=NaN(4,nb_blocks);
% resume_score_odors=NaN(4,nb_odors,nb_blocks);
% mean_licking=NaN(2,nb_blocks);
% mean_licking_odors=NaN(2,nb_odors,nb_blocks);
% mean_nb_licking=NaN(2,nb_blocks);
% mean_nb_licking_odors=NaN(2,nb_odors,nb_blocks);
% 
% for b=1:nb_blocks
%     correct_score(b)=(sum(scorer(:,b) == 1)+sum(scorer(:,b) == 4))/length(scorer(:,b));
%     resume_score(:,b)=[sum(scorer(:,b) == 1) sum(scorer(:,b) == 2) sum(scorer(:,b) == 3) sum(scorer(:,b) == 4)];
%     mean_licking(:,b)=[nansum(duration_licks(((scorer(:,b) == 1) | (scorer(:,b) == 2)),b))/(nb_trials_per_block/nb_odors) nansum(duration_licks(((scorer(:,b) == 3) | (scorer(:,b) == 3)),b))/(nb_trials_per_block/nb_odors)];
%     mean_nb_licking(:,b)=[nanmean(nb_licks(((scorer(:,b) == 1) | (scorer(:,b) == 2)),b)) nanmean(nb_licks(((scorer(:,b) == 3) | (scorer(:,b) == 3)),b))];
%     for o=1:nb_odors
%         scorer_odor=(trial_info.odor_identity(1:nb_trials_per_block,b) == o).*scorer(1:nb_trials_per_block,b);
%            correct_score_odors(o,b)=(sum(scorer_odor == 1)+sum(scorer_odor == 4))/sum(scorer_odor~=0);
%         resume_score_odors(:,o,b)=[sum(scorer_odor == 1) sum(scorer_odor == 2) sum(scorer_odor == 3) sum(scorer_odor == 4)];
%         mean_licking_odors(:,o,b)=[nansum(duration_licks(((scorer_odor == 1) | (scorer_odor == 2)),b))/sum(scorer_odor~=0) nansum(duration_licks(((scorer_odor == 3) | (scorer_odor == 3)),b))/sum(scorer_odor~=0)];
%         mean_nb_licking_odors(:,o,b)=[nanmean(nb_licks(((scorer_odor == 1) | (scorer_odor == 2)),b)) nanmean(nb_licks(((scorer_odor == 3) | (scorer_odor == 3)),b))];
%     end
% end

% figure('Position',[100 100 800 800]);
% subplot(2,2,1);
% plot(correct_score);
% ylim([0 1]);
% title('All odors');
% 
% subplot(2,2,2);
% bar(transpose(resume_score),'stacked');
% 
% subplot(2,2,3);
% bar(transpose(mean_licking));
% 
% hgexport(gcf,'session_performance_ALLODORS.png',hgexport('readstyle','docs'),'Format','png');
% 
% for o=1:nb_odors
%     figure('Position',[100 100 800 800]);
%     subplot(2,2,1);
%     plot(correct_score_odors(o,:));
%     ylim([0 1]);
%     title(odors.name{o,1});
%     
%     subplot(2,2,2);
%     bar(transpose(squeeze(resume_score_odors(:,o,:))),'stacked');
%     
%     subplot(2,2,3);
%     bar(transpose(squeeze(mean_licking_odors(:,o,:))));
%     
%     hgexport(gcf,strcat('session_performance_',odors.name{o,1},'.png'),hgexport('readstyle','docs'),'Format','png');
% end