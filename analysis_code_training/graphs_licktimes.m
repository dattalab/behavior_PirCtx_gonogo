type = 1;

figure
hold on
errorbar(stat_conc_allm_mean_licking_latency_odors.mean_var(8,1:conc_allm_nb_min_true_blocks),stat_conc_allm_mean_licking_latency_odors.SEM(8,1:conc_allm_nb_min_true_blocks)/2,'Color','magenta')
errorbar(stat_conc_allm_mean_licking_latency_odors.mean_var(4,1:conc_allm_nb_min_true_blocks),stat_conc_allm_mean_licking_latency_odors.SEM(4,1:conc_allm_nb_min_true_blocks)/2,'Color','cyan')
%plot(stat_conc_allm_correct_score_sep.median_var(1,1:conc_allm_nb_min_true_blocks),'Color','green')
%plot(stat_conc_allm_correct_score_sep.median_var(2,1:conc_allm_nb_min_true_blocks),'Color','blue')
if(type == 1)
    legend('2-AMINOACETOPHENONE 1:100 (+,vs)','A-IONONE 1:100 (+,&)','Location','southeast') % impair
else
    legend('A-IONONE 1:100 (+,vs)','2-AMINOACETOPHENONE 1:100 (+,&)','Location','southeast') % pair
end
legend('boxoff')
xlabel('Block #')
ylabel('Mean licking latency')
hgexport(gcf,'15o-impair_conc_lickingtimes.png',hgexport('readstyle','docs'),'Format','png');

[h,p,ci,stats]=ttest(stat_conc_allm_mean_licking_latency_odors.mean_var(8,1:conc_allm_nb_min_true_blocks),stat_conc_allm_mean_licking_latency_odors.mean_var(4,1:conc_allm_nb_min_true_blocks));
