clear all

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

disp('# Files concatenation')
[~, deepestFolder, ~] = fileparts(pwd);
FileName=0;
while(isequal(FileName,0))
    [FileName,PathName] = uiputfile('*.mat','Save concatenated file as',fullfile(pwd,strcat(deepestFolder,'.mat')));
end

all_odor_identity=[];
all_odors={};

for i=1:nb_datafiles
    fprintf('Processing %s...\n',fullfile(list_datafiles{i,2},list_datafiles{i,1}));
    load(fullfile(list_datafiles{i,2},list_datafiles{i,1}),'odors','trial_info');
    real_b=0;
    for b=1:size(trial_info.odor_identity,2)
        if(sum(trial_info.odor_identity(:,b) == 0) == 0)
            real_b=real_b+1;
            all_odor_identity=horzcat(all_odor_identity,trial_info.odor_identity(:,b));
            all_odors{real_b,i}=odors;
        end
    end
    clear odors trial_info
end

save(fullfile(PathName,FileName),'all_*','list_datafiles');

csvwrite(strrep(fullfile(PathName,FileName),'.mat','_trials.csv'),all_odor_identity);

T=cell(length(all_odors{1,1}.valence(:,1)),3);
for o=1:length(all_odors{1,1}.valence(:,1))
    T(o,:)=[o,{all_odors{1,1}.name{o,1}},{all_odors{1,1}.valence(o,1)}];
end
cell2csv(strrep(fullfile(PathName,FileName),'.mat','_odorlist.csv'),T);
clear T

clear all