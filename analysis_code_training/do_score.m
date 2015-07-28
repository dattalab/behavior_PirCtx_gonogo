function [nb_blocks,nb_trials_per_block,nb_odors] = do_score(pathname,filename,do_graphics)
load(fullfile(pathname,filename));
nb_blocks_data=length(block_param.nb_trials);
nb_trials_per_block=max(block_param.nb_trials);
nb_odors=size(odors.valence,1);

if(size(lick_events,1) < nb_trials_per_block)
    for i=size(lick_events,1):nb_trials_per_block
        lick_events{i,1}=[];
    end
end

blocks=NaN(1,nb_blocks_data);
nb_blocks=0;
for b=1:nb_blocks_data
    if(nansum(trial_info.odor_identity(:,b) == 0) == 0)
        nb_blocks=nb_blocks+1;
        blocks(b)=nb_blocks;
    end
end

scorer=NaN(nb_trials_per_block,nb_blocks);
nb_licks=NaN(nb_trials_per_block,nb_blocks);

for b=1:nb_blocks_data
    if(~isnan(blocks(b)))
        rb=blocks(b);
        for t=1:nb_trials_per_block
            if(odors.valence(trial_info.odor_identity(t,b),b) == 1)
                if(size(lick_events{t,b},1) > 0)
                    if(length(find((lick_events{t,b}(:,1) < 2000))) > 0)
                        scorer(t,rb)=1;
                    else
                        scorer(t,rb)=2;
                    end
                    nb_licks(t,rb)=length(find((lick_events{t,b}(:,1) < 2000)));
                else
                    scorer(t,rb)=2;
                    nb_licks(t,rb)=0;
                end
            elseif(odors.valence(trial_info.odor_identity(t,b),b) == 0)
                if(size(lick_events{t,b},1) > 0)
                    if(length(find((lick_events{t,b}(:,1) < 2000))) > 0)
                        scorer(t,rb)=3;
                    else
                        scorer(t,rb)=4;
                    end
                    nb_licks(t,rb)=length(find((lick_events{t,b}(:,1) < 2000)));
                else
                    scorer(t,rb)=4;
                    nb_licks(t,rb)=0;
                end
            end
        end
    end
end

duration_licks=NaN(nb_trials_per_block,nb_blocks);
licking_latency=NaN(nb_trials_per_block,nb_blocks);
for b=1:nb_blocks_data
    if(~isnan(blocks(b)))
        rb=blocks(b);
        for t=1:nb_trials_per_block
            duration_licks(t,rb)=0;
            if(size(lick_events{t,b},1) > 0)
                row=[];
                [row, col]=find((lick_events{t,b}(:,1) < 2000));
                licking_latency(t,rb)=lick_events{t,b}(1,1);
                if(length(row) > 0)
                    for i=1:length(row)
                        if((lick_events{t,b}(row(i),2) < 2000) && (lick_events{t,b}(row(i),2) > 0))
                            end_lick=lick_events{t,b}(row(i),2);
                        else
                            end_lick=2000;
                        end
                        duration_licks(t,rb)=duration_licks(t,rb)+(end_lick-lick_events{t,b}(row(i),1));
                    end
                end
            end
        end
    end
end

correct_score=NaN(1,nb_blocks);
correct_score_sep=NaN(2,nb_blocks);
correct_score_odors=NaN(nb_odors,nb_blocks);
resume_score=NaN(4,nb_blocks);
resume_score_odors=NaN(4,nb_odors,nb_blocks);

mean_licking=NaN(2,nb_blocks);
median_licking=NaN(2,nb_blocks);
mean_licking_latency=NaN(2,nb_blocks);
median_licking_latency=NaN(2,nb_blocks);

mean_licking_odors=NaN(nb_odors,nb_blocks);
median_licking_odors=NaN(nb_odors,nb_blocks);
mean_licking_latency_odors=NaN(nb_odors,nb_blocks);
median_licking_latency_odors=NaN(nb_odors,nb_blocks);

mean_licking_odors_sep=NaN(2,nb_odors,nb_blocks);
median_licking_odors_sep=NaN(2,nb_odors,nb_blocks);
mean_licking_latency_odors_sep=NaN(2,nb_odors,nb_blocks);
median_licking_latency_odors_sep=NaN(2,nb_odors,nb_blocks);

mean_nb_licking=NaN(2,nb_blocks);
mean_nb_licking_odors=NaN(nb_odors,nb_blocks);
mean_nb_licking_odors_sep=NaN(2,nb_odors,nb_blocks);

for b=1:nb_blocks_data
    if(~isnan(blocks(b)))
        rb=blocks(b);
        correct_score(rb)=(sum(scorer(:,rb) == 1)+sum(scorer(:,rb) == 4))/length(scorer(:,rb));
        correct_score_sep(:,rb)=[sum(scorer(:,rb) == 1)/sum((scorer(:,rb) == 1) | (scorer(:,rb) == 2)) sum(scorer(:,rb) == 4)/sum((scorer(:,rb) == 3) | (scorer(:,rb) == 4))];
        resume_score(:,rb)=[sum(scorer(:,rb) == 1) sum(scorer(:,rb) == 2) sum(scorer(:,rb) == 3) sum(scorer(:,rb) == 4)];
        mean_licking(:,rb)=[nansum(duration_licks((scorer(:,rb) == 1),rb))/sum(scorer(:,rb) == 1 | scorer(:,rb) == 2) nansum(duration_licks((scorer(:,rb) == 3),rb))/sum(scorer(:,rb) == 3 | scorer(:,rb) == 4)];
        median_licking(:,rb)=[nanmedian(duration_licks(((scorer(:,rb) == 1) | (scorer(:,rb) == 2)),rb)) nanmedian(duration_licks(((scorer(:,rb) == 3) | (scorer(:,rb) == 4)),rb))];
        mean_licking_latency(:,rb)=[nansum(licking_latency((scorer(:,rb) == 1),rb))/sum(scorer(:,rb) == 1 | scorer(:,rb) == 2) nansum(licking_latency((scorer(:,rb) == 3),rb))/sum(scorer(:,rb) == 3 | scorer(:,rb) == 4)];
        median_licking_latency(:,rb)=[nanmedian(licking_latency(((scorer(:,rb) == 1) | (scorer(:,rb) == 2)),rb)) nanmedian(licking_latency(((scorer(:,rb) == 3) | (scorer(:,rb) == 4)),rb))];
        mean_nb_licking(:,rb)=[nanmean(nb_licks((scorer(:,rb) == 1 | scorer(:,rb) == 2),rb)) nanmean(nb_licks(((scorer(:,rb) == 3) | (scorer(:,b) == 4)),rb))];
        for o=1:nb_odors
            if(~isempty(odors.name(o,b)))
                scorer_odor=(trial_info.odor_identity(1:nb_trials_per_block,b) == o).*scorer(1:nb_trials_per_block,rb);
                correct_score_odors(o,rb)=(sum(scorer_odor == 1)+sum(scorer_odor == 4))/sum(scorer_odor~=0);
                resume_score_odors(:,o,rb)=[sum(scorer_odor == 1) sum(scorer_odor == 2) sum(scorer_odor == 3) sum(scorer_odor == 4)];
                mean_licking_odors_sep(:,o,rb)=[nansum(duration_licks(((scorer_odor == 1) | (scorer_odor == 2)),rb))/nansum(scorer_odor~=0) nansum(duration_licks(((scorer_odor == 3) | (scorer_odor == 4)),rb))/nansum(scorer_odor~=0)];
                median_licking_odors_sep(:,o,rb)=[nanmedian(duration_licks(((scorer_odor == 1) | (scorer_odor == 2)),rb)) nansum(duration_licks(((scorer_odor == 3) | (scorer_odor == 4)),rb))];
                mean_licking_latency_odors_sep(:,o,rb)=[nansum(licking_latency(((scorer_odor == 1) | (scorer_odor == 2)),rb))/nansum(scorer_odor~=0) nansum(licking_latency(((scorer_odor == 3) | (scorer_odor == 4)),rb))/nansum(scorer_odor~=0)];
                median_licking_latency_odors_sep(:,o,rb)=[nanmedian(licking_latency(((scorer_odor == 1) | (scorer_odor == 2)),rb)) nansum(licking_latency(((scorer_odor == 3) | (scorer_odor == 4)),rb))];
                mean_nb_licking_odors_sep(:,o,rb)=[nanmean(nb_licks(((scorer_odor == 1) | (scorer_odor == 2)),rb)) nanmean(nb_licks(((scorer_odor == 3) | (scorer_odor == 4)),rb))];
                
                mean_licking_odors(o,rb)=nansum(duration_licks((scorer_odor~=0),rb))/nansum(scorer_odor~=0);
                median_licking_odors(o,rb)=nanmedian(duration_licks((scorer_odor~=0),rb));
                mean_licking_latency_odors(o,rb)=nansum(licking_latency((scorer_odor~=0),rb))/nansum(scorer_odor~=0);
                median_licking_latency_odors(o,rb)=nanmedian(licking_latency((scorer_odor~=0),rb));
                mean_nb_licking_odors(o,rb)=nanmean(nb_licks((scorer_odor~=0),rb));
                
            end
        end
    end
end

odor_identity=trial_info.odor_identity;

if(iscell(iti))
    new_iti=NaN(nb_trials_per_block,nb_blocks);
    for b=1:nb_blocks_data
        if(~isnan(blocks(b)))
            rb=blocks(b);
            for t=1:nb_trials_per_block
                if(~isempty(iti{t,b}))
                    new_iti(t,rb)=iti{t,b}(1);
                else
                    new_iti(t,rb)=NaN;
                end
            end
        end
    end
    clear iti
    iti=new_iti;
end

save(fullfile(pathname,strrep(filename,'.mat','_results.mat')),'nb_blocks','nb_trials_per_block','nb_odors','odor_identity','odors','iti','scorer','nb_licks','duration_licks','licking_latency','correct_score','correct_score_sep','correct_score_odors','resume_score','resume_score_odors','mean_licking','median_licking','mean_licking_odors','mean_licking_odors_sep','median_licking_odors','median_licking_odors_sep','mean_licking_latency','median_licking_latency','mean_licking_latency_odors','mean_licking_latency_odors_sep','median_licking_latency_odors','median_licking_latency_odors_sep','mean_nb_licking','mean_nb_licking_odors','mean_nb_licking_odors_sep');

if(do_graphics == 1)
    figure('Position',[100 100 800 800]);
    subplot(2,2,1);
    plot(correct_score);
    ylim([0 1]);
    title('All odors');
    
    subplot(2,2,2);
    bar(transpose(resume_score),'stacked');
    
    subplot(2,2,3);
    bar(transpose(mean_licking));
    
    hgexport(gcf,fullfile(pathname,'session_performance__ALLODORS.png'),hgexport('readstyle','docs'),'Format','png');
    
    for o=1:nb_odors
        figure('Position',[100 100 800 800]);
        subplot(2,2,1);
        plot(correct_score_odors(o,:));
        ylim([0 1]);
        title(odors.name{o,1});
        
        subplot(2,2,2);
        bar(transpose(squeeze(resume_score_odors(:,o,:))),'stacked');
        
        subplot(2,2,3);
        bar(transpose(squeeze(mean_licking_odors_sep(:,o,:))));
        
        hgexport(gcf,fullfile(pathname,strcat('session_performance_',odors.name{o,1},'.png')),hgexport('readstyle','docs'),'Format','png');
    end
    close all
end
end

