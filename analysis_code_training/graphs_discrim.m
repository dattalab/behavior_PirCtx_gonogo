clear all

type=1; % impair = 1; pair =2
FileName=0;
while(FileName == 0)
    [FileName,PathName] = uigetfile('*.mat',strcat('Select final data file'));
end

load(fullfile(PathName,FileName))

% impair : 7/8 - 3,4
% pair : 3, 4 - 7,8
if(type == 1)
    couple_discrim_odors=[7,8];
    couple_gene_odors=[3,4];
else
    couple_discrim_odors=[3,4];
    couple_gene_odors=[7,8];
end

coupledis_conc_allm_correct_score_odors=nanmean(conc_allm_correct_score_odors(couple_discrim_odors,:,:),1);
stat_coupledis_conc_allm_correct_score_odors=do_stats_mice(coupledis_conc_allm_correct_score_odors);
couplegene_conc_allm_correct_score_odors=nanmean(conc_allm_correct_score_odors(couple_gene_odors,:,:),1);
stat_couplegene_conc_allm_correct_score_odors=do_stats_mice(couplegene_conc_allm_correct_score_odors);
stat_conc_allm_correct_score_odors=do_stats_mice(conc_allm_correct_score_odors);

% Overall performance plot discrim
figure
hold on
errorbar(stat_coupledis_conc_allm_correct_score_odors.median_var(1:conc_allm_nb_min_true_blocks),stat_coupledis_conc_allm_correct_score_odors.IQR(1:conc_allm_nb_min_true_blocks)/2,'Color','black')
line([0 conc_allm_nb_min_true_blocks],[0.5 0.5],'Color','red','LineStyle','--')
ylim([0 1.1])
xlabel('Block #')
ylabel('Median block performance')
hgexport(gcf,fullfile(PathName,strrep(FileName,'.mat','_discriminperformance.png')),hgexport('readstyle','docs'),'Format','png');

% Overall performance plot discrim + gene
figure
hold on
errorbar(stat_coupledis_conc_allm_correct_score_odors.median_var(1:conc_allm_nb_min_true_blocks),stat_coupledis_conc_allm_correct_score_odors.IQR(1:conc_allm_nb_min_true_blocks)/2,'Color','magenta','LineWidth',2.5)
errorbar(stat_couplegene_conc_allm_correct_score_odors.median_var(1:conc_allm_nb_min_true_blocks),stat_couplegene_conc_allm_correct_score_odors.IQR(1:conc_allm_nb_min_true_blocks)/2,'Color','cyan','LineWidth',1)
line([0 conc_allm_nb_min_true_blocks],[0.5 0.5],'Color','red','LineStyle','--')
if(type == 1)
    legend('2-AMINOACETOPHENONE (1:100 vs 1:1)','A-IONONE (1:100 & 1:1)','Location','southeast') % impair
else
    legend('A-IONONE (1:100 vs 1:1)','2-AMINOACETOPHENONE (1:100 & 1:1)','Location','southeast') % pair
end
ylim([0 1.1])
xlim([0 21])
xlabel('Block #')
ylabel('Median block performance')
hgexport(gcf,fullfile(PathName,strrep(FileName,'.mat','_discrimingeneperformance.png')),hgexport('readstyle','docs'),'Format','png');
[h,p,ci,stats] = ttest(stat_coupledis_conc_allm_correct_score_odors.mean_var(1:7),stat_couplegene_conc_allm_correct_score_odors.mean_var(1:7));
[h,p,ci,stats] = ttest(stat_coupledis_conc_allm_correct_score_odors.mean_var(43:47),stat_couplegene_conc_allm_correct_score_odors.mean_var(43:47));

% Discrim vs gene (1:100)
figure
hold on
ylim([0 1.1])
errorbar(stat_conc_allm_correct_score_odors.median_var(couple_discrim_odors(2),1:conc_allm_nb_min_true_blocks),stat_conc_allm_correct_score_odors.IQR(couple_discrim_odors(2),1:conc_allm_nb_min_true_blocks)/2,'Color','magenta','LineWidth',2.5)
errorbar(stat_conc_allm_correct_score_odors.median_var(couple_gene_odors(2),1:conc_allm_nb_min_true_blocks),stat_conc_allm_correct_score_odors.IQR(couple_gene_odors(2),1:conc_allm_nb_min_true_blocks)/2,'Color','cyan','LineWidth',1)
%plot(stat_conc_allm_correct_score_sep.median_var(1,1:conc_allm_nb_min_true_blocks),'Color','green')
%plot(stat_conc_allm_correct_score_sep.median_var(2,1:conc_allm_nb_min_true_blocks),'Color','blue')
line([0 conc_allm_nb_min_true_blocks],[0.5 0.5],'Color','red','LineStyle','--')
if(type == 1)
    legend('2-AMINOACETOPHENONE 1:100 (+,vs)','A-IONONE 1:100 (+,&)','Location','southeast') % impair
else
    legend('A-IONONE 1:100 (+,vs)','2-AMINOACETOPHENONE 1:100 (+,&)','Location','southeast') % pair
end
legend('boxoff')
xlabel('Block #')
ylabel('Median block performance')
xlim([0 14])
hgexport(gcf,fullfile(PathName,strrep(FileName,'.mat','_1100performancebroke.png')),hgexport('readstyle','docs'),'Format','png');

figure
hold on
ylim([0 1.1])
errorbar(stat_conc_allm_correct_score_odors.median_var(couple_discrim_odors(1),1:conc_allm_nb_min_true_blocks),stat_conc_allm_correct_score_odors.IQR(couple_discrim_odors(1),1:conc_allm_nb_min_true_blocks)/2,'Color','magenta','LineWidth',2.5)
errorbar(stat_conc_allm_correct_score_odors.median_var(couple_gene_odors(1),1:conc_allm_nb_min_true_blocks),stat_conc_allm_correct_score_odors.IQR(couple_gene_odors(1),1:conc_allm_nb_min_true_blocks)/2,'Color','cyan','LineWidth',1)
line([0 conc_allm_nb_min_true_blocks],[0.5 0.5],'Color','red','LineStyle','--')
if(type == 1)
    legend('2-AMINOACETOPHENONE 1:1 (-,vs)','A-IONONE 1:1 (-,&)','Location','southeast') % impair
else
    legend('A-IONONE 1:1 (-,vs)','2-AMINOACETOPHENONE 1:1 (-,&)','Location','southeast') % pair
end
legend('boxoff')
xlabel('Block #')
ylabel('Median block performance')
xlim([0 14])
hgexport(gcf,fullfile(PathName,strrep(FileName,'.mat','_11performancebroke.png')),hgexport('readstyle','docs'),'Format','png');


% CS+ vs CS- performance plot (discrim)
figure
hold on
ylim([0 1.1])
errorbar(stat_conc_allm_correct_score_odors.median_var(couple_discrim_odors(2),1:conc_allm_nb_min_true_blocks),stat_conc_allm_correct_score_odors.IQR(couple_discrim_odors(2),1:conc_allm_nb_min_true_blocks)/2,'Color','green','LineWidth',2.5)
errorbar(stat_conc_allm_correct_score_odors.median_var(couple_discrim_odors(1),1:conc_allm_nb_min_true_blocks),stat_conc_allm_correct_score_odors.IQR(couple_discrim_odors(1),1:conc_allm_nb_min_true_blocks)/2,'Color','blue','LineWidth',1)
line([0 conc_allm_nb_min_true_blocks],[0.5 0.5],'Color','red','LineStyle','--')
if(type == 1)
    legend('2-AMINOACETOPHENONE 1:100 (+)','2-AMINOACETOPHENONE 1:1 (-)','Location','southeast') % impair
else
    legend('A-IONONE 1:100 (+)','A-IONONE 1:1 (-)','Location','southeast') % pair
end
legend('boxoff')
xlabel('Block #')
ylabel('Median block performance')
hgexport(gcf,fullfile(PathName,strrep(FileName,'.mat','_discriminperformancebroke.png')),hgexport('readstyle','docs'),'Format','png');
