% CS+ vs CS- performance plot (discrim)
couple_discrim_odors=[7,8];

figure
hold on
plot(reshape(all_mean_licking(1,:,:),[1,45]),'Color','green')
plot(reshape(all_mean_licking(2,:,:),[1,45]),'Color','blue')
legend('ALPHA-PINENE (+)','PROPYLBUTYRATE (-)')
legend('boxoff')
xlabel('Block #')
ylabel('Mean licking duration (ms)')
hgexport(gcf,'M167-2o-lickingbroken.png',hgexport('readstyle','docs'),'Format','png');

figure
hold on
plot(reshape(all_mean_licking_latency(1,:,:),[1,45]),'Color','green')
legend('CS+','CS-')
legend('boxoff')
xlabel('Block #')
ylabel('Mean licking latency (ms)')
hgexport(gcf,'M167-2o-lickinglatbroken.png',hgexport('readstyle','docs'),'Format','png');
