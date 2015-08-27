clear all

FileName=0;
while(FileName == 0)
    [FileName,PathName] = uigetfile('*.mat',strcat('Select final data file'));
end

load(fullfile(PathName,FileName))

% Overall performance plot
figure;
errorbar(stat_conc_allm_correct_score.median_var(1:conc_allm_nb_min_true_blocks),stat_conc_allm_correct_score.IQR(1:conc_allm_nb_min_true_blocks)/2,'Color','black')
line([0 conc_allm_nb_min_true_blocks],[0.85 0.85],'Color','red','LineStyle','--')
ylim([0 1])
xlabel('Block #')
ylabel('Median block performance')

hgexport(gcf,fullfile(PathName,strrep(FileName,'.mat','_overallperformance.png')),hgexport('readstyle','docs'),'Format','png');


% CS+ vs CS- performance plot
figure
hold on
ylim([0 1])
errorbar(stat_conc_allm_correct_score_sep.median_var(1,1:conc_allm_nb_min_true_blocks),stat_conc_allm_correct_score_sep.IQR(1,1:conc_allm_nb_min_true_blocks)/2,'Color','green')
errorbar(stat_conc_allm_correct_score_sep.median_var(2,1:conc_allm_nb_min_true_blocks),stat_conc_allm_correct_score_sep.IQR(2,1:conc_allm_nb_min_true_blocks)/2,'Color','blue')
%plot(stat_conc_allm_correct_score_sep.median_var(1,1:conc_allm_nb_min_true_blocks),'Color','green')
%plot(stat_conc_allm_correct_score_sep.median_var(2,1:conc_allm_nb_min_true_blocks),'Color','blue')
line([0 conc_allm_nb_min_true_blocks],[0.85 0.85],'Color','red','LineStyle','--')
legend('CS+','CS-','Location','southeast')
legend('boxoff')
xlabel('Block #')
ylabel('Median block performance')
hgexport(gcf,fullfile(PathName,strrep(FileName,'.mat','_overallperformancebroke.png')),hgexport('readstyle','docs'),'Format','png');

% CS+ vs CS- licking duration plot
figure
hold on
errorbar(stat_conc_allm_mean_licking.mean_var(1,1:conc_allm_nb_min_true_blocks),stat_conc_allm_mean_licking.SEM(1,1:conc_allm_nb_min_true_blocks)/2,'Color','green')
errorbar(stat_conc_allm_mean_licking.mean_var(2,1:conc_allm_nb_min_true_blocks),stat_conc_allm_mean_licking.SEM(2,1:conc_allm_nb_min_true_blocks)/2,'Color','blue')
legend('CS+','CS-','Location','southeast')
legend('boxoff')
xlabel('Block #')
ylabel('Mean licking duration')
hgexport(gcf,fullfile(PathName,strrep(FileName,'.mat','_overalllickingtimebroke.png')),hgexport('readstyle','docs'),'Format','png');