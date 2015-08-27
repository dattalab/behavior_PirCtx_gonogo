function [ final_var ] = do_stats_mice( var )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

dim_mice=ndims(var);
nb_mice=size(var,dim_mice);
mean_var=nanmean(var,dim_mice);
median_var=nanmedian(var,dim_mice);
SEM = nanstd(var,1,dim_mice)/sqrt(nb_mice);               % Standard Error
ts = tinv([0.025  0.975],nb_mice-1);      % T-Score
CI_low = mean_var + ts(1)*SEM;                      % Confidence Intervals
CI_high = mean_var + ts(2)*SEM;
CI=squeeze(cat(ndims(CI_low)+1,CI_low,CI_high));
CI_amplitude=CI_high-CI_low;
MAD=mad(var,1,dim_mice);
IQR=iqr(var,dim_mice);

final_var=struct;
final_var.mean_var=mean_var;
final_var.median_var=median_var;
final_var.SEM=SEM;
final_var.CI=CI;
final_var.CI_low=CI_low;
final_var.CI_high=CI_high;
final_var.CI_amplitude=CI_amplitude;
final_var.MAD=MAD;
final_var.IQR=IQR;

end

