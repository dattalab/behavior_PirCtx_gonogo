function varargout = gui_getlines(varargin)
% GUI_GETLINES MATLAB code for gui_getlines.fig
%      GUI_GETLINES, by itself, creates a new GUI_GETLINES or raises the existing
%      singleton*.
%
%      H = GUI_GETLINES returns the handle to a new GUI_GETLINES or the handle to
%      the existing singleton*.
%
%      GUI_GETLINES('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI_GETLINES.M with the given input arguments.
%
%      GUI_GETLINES('Property','Value',...) creates a new GUI_GETLINES or raises the
%      existing singleton*.  Starting from thse left, property value pairs are
%      applied to the GUI before gui_getlines_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to gui_getlines_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help gui_getlines

% Last Modified by GUIDE v2.5 21-Apr-2015 21:42:47

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @gui_getlines_OpeningFcn, ...
                   'gui_OutputFcn',  @gui_getlines_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before gui_getlines is made visible.
function gui_getlines_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to gui_getlines (see VARARGIN)

% Choose default command line output for gui_getlines
handles.output = hObject;

set(get(handles.lickingRatePlot,'XLabel'),'String','time [s]');
set(get(handles.lickingRatePlot,'YLabel'),'String','licking frequency [hz]');
set(get(handles.lastTrialPlot,'XLabel'),'String','time [s]');
set(get(handles.lastTrialPlot,'YLabel'),'String','licking frequency [hz]');
set(get(handles.currentTrialPlot,'XLabel'),'String','time [s]');
set(get(handles.currentTrialPlot,'YLabel'),'String','licking frequency [hz]');

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes gui_getlines wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = gui_getlines_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

function []=do_acquisition(handles)
global last_update, global tStart, global running_state, global s;
tStart=tic;
last_update=toc(tStart);

delete(instrfindall);

% Choose filenames
[filename,path]=uiputfile('*.txt','Saving experiment log',['log_' datestr(now,'yymmdd_HHMM') '_serial.txt']);
savefile_log=fullfile(path,filename);
[filename,path]=uiputfile('*.txt','Saving treatment log',['log_' datestr(now,'yymmdd_HHMM') '_treatment.txt']);
savefile_treatlog=fullfile(path,filename);
[filename,path]=uiputfile('*.mat','Saving data',['log_' datestr(now,'yymmdd_HHMM') '_data.mat']);
savefile_data=fullfile(path,filename);
clear filename path

% Open log files
logfile=fopen(savefile_log,'w');
treatlogfile=fopen(savefile_treatlog,'w');

% Initialize communication with Arduino
s=serial('/dev/cu.usbmodem1431');
fopen(s);
%s.BytesAvailable=1;

% Initialize variables
progression=struct;
progression.init_duration=[];
progression.assessment_window=[];
progression.init_odors=[];
logmode_only=0;
odors=struct;
odors.name={};
odors.valence=[];
odors.valve=[];
block_param=struct;
block_param.nb_trials=[];
block_param.nb_odors=[];
block_param.start_assessment_window=[];
block_param.duration_assessment_window=[];
trial_info=struct;
trial_info.odor_identity=[0 0];
trial_info.start_time=[0 0];
lick_events_raw={};
lick_events={};
us_events_raw={};
us_events={};
current_block_id=0;
current_trial=0;
running_state=1;
last_line='/';
connexion_ok=0;
past_trials=[];
performance_hitrate=[];
score_trials=[];

while(running_state > 0)
    if(running_state == 2)
        drawnow
    end
    if(strcmp(last_line,'KILL'))
        running_state=0;
    elseif(s.BytesAvailable > 0)
        last_line_n=fgetl(s);
        %last_line_n=fgets(logfile);
        last_line=last_line_n(1:end-1);
        if(strcmp(last_line(1:4),'KILL'))
            running_state=0;
        elseif(~strcmp(last_line(1),'/') && logmode_only == 0)
            fwrite(logfile,sprintf('%s\n',last_line));
            split_last_line=strsplit(last_line,',');
            if(strcmp(split_last_line{2},'CONOK'))
                if(strcmp(split_last_line{3},'1'))
                    connexion_ok=1;
                end
            elseif(connexion_ok == 0)
                logmode_only=1;
                disp('Error: Ethernet connexion is not ready.');
            elseif(strcmp(split_last_line{2},'BIP'))
                current_block_id=str2num(split_last_line{3});
                block_param.nb_trials(current_block_id)=str2num(split_last_line{4});
                block_param.nb_odors(current_block_id)=str2num(split_last_line{5});
                dualfprintf(treatlogfile,'>>> BLOCK %d\n',current_block_id);
                dualfprintf(treatlogfile,'>> Block initialization:\nNb of trials: %d / Nb of odors: %d\n',block_param.nb_trials(current_block_id),block_param.nb_odors(current_block_id));
                progression.init_duration(current_block_id)=0;
                progression.init_assessment_window(current_block_id)=0;
                progression.init_odors(current_block_id)=0;
                
                set(handles.textBlockID,'String',sprintf('Block #%d',current_block_id));
                set(handles.textTrialCount,'String',sprintf('0/%d',block_param.nb_trials(current_block_id)));
                current_odor_table=cell(block_param.nb_odors(current_block_id),3);
            elseif(current_block_id == 0 || str2num(split_last_line{3}) ~= current_block_id)
                dualfprintf(treatlogfile,'Error: does not match current block id. Command non treated: %s.',last_line);
            elseif((strcmp(split_last_line{2},'BIW')) && (progression.init_assessment_window(current_block_id) == 0))
                block_param.start_assessment_window(current_block_id)=str2num(split_last_line{4});
                block_param.duration_assessment_window(current_block_id)=str2num(split_last_line{5});
                progression.init_assessment_window(current_block_id)=1;
            elseif((strcmp(split_last_line{2},'BID')) && (progression.init_duration(current_block_id) == 0))
                block_param.duration_odor_sampling(current_block_id)=str2num(split_last_line{4});
                block_param.duration_wait(current_block_id)=str2num(split_last_line{5});
                block_param.duration_outcome(current_block_id)=str2num(split_last_line{6});
                block_param.duration_interstimulus(current_block_id)=str2num(split_last_line{7});
                dualfprintf(treatlogfile,'>> Duration initialization:\nOdor sampling (%d ms) - Waiting time (%d ms) - Outcome (%d ms) - ISI (%d ms)\n',block_param.duration_odor_sampling(current_block_id),block_param.duration_wait(current_block_id),block_param.duration_outcome(current_block_id),block_param.duration_interstimulus(current_block_id));
                progression.init_duration(current_block_id)=1;
            elseif((strcmp(split_last_line{2},'BIO')) && (progression.init_odors(current_block_id) == 0))
                odor_id=str2num(split_last_line{4});
                odors.name{odor_id,current_block_id}=split_last_line{5};
                odors.valence(odor_id,current_block_id)=str2num(split_last_line{6});
                odors.valve(odor_id,current_block_id)=str2num(split_last_line{7});
                dualfprintf(treatlogfile,'> Odor %d: %s // Val=%d; Valve=%d\n',odor_id,odors.name{odor_id,current_block_id},odors.valence(odor_id,current_block_id),odors.valve(odor_id,current_block_id));
                if(sum(odors.valve(1:end,current_block_id) ~= 0) == block_param.nb_odors(current_block_id))
                    progression.init_odors(current_block_id)=1;
                end
                current_odor_table(odor_id,:)={odors.name{odor_id,current_block_id},odors.valence(odor_id,current_block_id),odors.valve(odor_id,current_block_id)};
                set(handles.currentBlockTable,'Data',current_odor_table);
            elseif(progression.init_duration(current_block_id) == 0 || progression.init_assessment_window(current_block_id) == 0 || progression.init_odors(current_block_id) == 0)
                logmode_only=1;
                dualfprintf(treatlogfile,'>> Could not acquire all the parameters before experiment starts. Starts in log-mode only.\n');
            elseif(strcmp(split_last_line{2},'O'))
                if(sum(size(past_trials)) ~= 0)
                    score_trials(past_trials(end,1),past_trials(end,2))=scoreTrial(current_trial_lickingData,[block_param.start_assessment_window(past_trials(end,2)) block_param.start_assessment_window(past_trials(end,2))+block_param.duration_assessment_window(past_trials(end,2))],odors.valence(trial_info.odor_identity(past_trials(end,1),past_trials(end,2)),past_trials(end,2)));
                    performance_hitrate(past_trials(end,1),past_trials(end,2))=mean(score_trials(1:past_trials(end,1),past_trials(end,2)) == 1);
                    updatePerformancePlot(performance_hitrate(:,past_trials(end,2)),handles);
                    updateTrialPlot('last',current_trial_lickingData,past_trials(end,1),past_trials(end,2),trial_info,odors,handles);
                end
                current_trial=str2num(split_last_line{4});
                trial_info.odor_identity(current_trial,current_block_id)=str2num(split_last_line{5});
                trial_info.start_time(current_trial,current_block_id)=str2num(split_last_line{1});
                lick_events_raw{current_block_id,current_trial}=[];
                lick_events{current_block_id,current_trial}=[];
                us_events_raw{current_block_id,current_trial}=[];
                us_events{current_block_id,current_trial}=[];
                dualfprintf(treatlogfile,'> Trial %d: %d/%s - T: %d\n',current_trial, trial_info.odor_identity(current_trial,current_block_id),odors.name{trial_info.odor_identity(current_trial,current_block_id),current_block_id},trial_info.start_time(current_trial,current_block_id));
                set(handles.currentTrialOdorText,'String',sprintf('#%d/%s      Val=%d; Valve=%d',current_trial,odors.name{trial_info.odor_identity(current_trial,current_block_id),current_block_id},odors.valence(trial_info.odor_identity(current_trial,current_block_id),current_block_id),odors.valve(trial_info.odor_identity(current_trial,current_block_id),current_block_id)));
                current_trial_lickingData=[];
                past_trials=[past_trials; current_trial current_block_id];
                last_update=0;
                updateTrialPlot('current',current_trial_lickingData,current_trial,current_block_id,trial_info,odors,handles);
            elseif(strcmp(split_last_line{2},'L'))
                lick_trial=str2num(split_last_line{4});
                lick_id=str2num(split_last_line{5});
                lick_status=str2num(split_last_line{6});
                lick_time_raw=str2num(split_last_line{1});
                lick_time=lick_time_raw-trial_info.start_time(lick_trial,current_block_id);
                if(lick_status == 0)
                    lick_events_raw{lick_trial,current_block_id}(lick_id,2)=lick_time_raw;
                    lick_events{lick_trial,current_block_id}(lick_id,2)=lick_time;
                    dualfprintf(treatlogfile,'Mouse licked - T: %d\n',lick_time);
                else
                    lick_events_raw{lick_trial,current_block_id}(lick_id,1)=lick_time_raw;
                    lick_events{lick_trial,current_block_id}(lick_id,1)=lick_time;
                    current_trial_lickingData(lick_id,1)=lick_time;
                    updateTrialPlot('current',current_trial_lickingData,current_trial,current_block_id,trial_info,odors,handles);
                end
            elseif(strcmp(split_last_line{2},'US'))
                us_trial=str2num(split_last_line{4});
                us_outcome_code=str2num(split_last_line{5});
                us_status=str2num(split_last_line{6});
                us_time_raw=str2num(split_last_line{1});
                us_time=us_time_raw-trial_info.start_time(us_trial,current_block_id);
                if(us_status == 1)
                    us_events_raw{us_trial,current_block_id}(1,1:2)=[us_time_raw us_outcome_code];
                    us_events{us_trial,current_block_id}(1,1:2)=[us_time us_outcome_code];
                end
            else
                dualfprintf(treatlogfile,'>> Unexpected syntax: %s\n',last_line);
            end
        else
            disp(last_line);
        end
        save(savefile_data,'block_param','lick_events','lick_events_raw','odors','trial_info','score_trials','performance_hitrate','us_events_raw','us_events');
    end
end

save(savefile_data,'block_param','lick_events','lick_events_raw','odors','trial_info','score_trials','performance_hitrate','us_events_raw','us_events');


fclose(s);
delete(s);
clear s

if(size(past_trials) ~= size(score_trials))
    score_trials(past_trials(end,1),past_trials(end,2))=scoreTrial(current_trial_lickingData,[block_param.start_assessment_window(past_trials(end,2)) block_param.start_assessment_window(past_trials(end,2))+block_param.duration_assessment_window(past_trials(end,2))],odors.valence(trial_info.odor_identity(past_trials(end,1),past_trials(end,2)),past_trials(end,2)));
    performance_hitrate(past_trials(end,1),past_trials(end,2))=mean(score_trials(1:past_trials(end,1),past_trials(end,2)) == 1);
    updatePerformancePlot(performance_hitrate(:,past_trials(end,2)),handles);
    updateTrialPlot('last',current_trial_lickingData,past_trials(end,1),past_trials(end,2),trial_info,odors,handles);
end

delete(instrfindall);
fclose(logfile);
fclose(treatlogfile);

set(handles.startButton,'Enable','on');

function []=updatePerformancePlot(data,handles)
axes(handles.blockPerformancePlot);
plot(handles.blockPerformancePlot,data);
xlim([0 10]);
ylim([0 1]);

function []=updateTrialPlot(type,licksData,current_trial,current_block_id,trial_info,odors,handles)
global last_update, global tStart;
switch(odors.valence(trial_info.odor_identity(current_trial,current_block_id),current_block_id))
    case 0
        color_line='black';
    case 1
        color_line='green';
    case 2
        color_line='red';
end
if(strcmp(type,'last'))
    set(handles.lastTrialOdorText,'String',sprintf('#%d/%s      Val=%d; Valve=%d',current_trial,odors.name{trial_info.odor_identity(current_trial,current_block_id),current_block_id},odors.valence(trial_info.odor_identity(current_trial,current_block_id),current_block_id),odors.valve(trial_info.odor_identity(current_trial,current_block_id),current_block_id)));
    id_plot=handles.lastTrialPlot;
else
    id_plot=handles.currentTrialPlot;
end
if(sum(size(licksData))>0)
    if((toc(tStart)-last_update) > 0.25)
        v=[0 transpose(1000./diff(licksData(:,1)))];
        axes(id_plot);
        stairs(licksData(:,1)/1000,v,color_line);
        xlim([0 4]);
        %ylim([0 10]);
        if(strcmp(type,'last'))
            axes(handles.lickingRatePlot);
            hold on
            stairs(licksData(:,1)/1000,v,color_line);
            xlim([0 4]);
            %ylim([0 10]);
        end
        last_update=toc(tStart);
    end
else
    axes(id_plot);
    stairs(0,0,color_line);
    xlim([0 4]);
    %ylim([0 10]);
end
drawnow

function score=scoreTrial(licksData,timeWindow,valence_trial)
% Hit = 1, Miss = 2, False Alarm = 3, Correct Rejection = 4
if(sum(size(licksData))>0)
    nb_licks=sum((licksData(:,1) >= timeWindow(1)) .* (licksData(:,1) <= timeWindow(2)));
else
    nb_licks=0;
end
if(nb_licks > 1)
    if(valence_trial == 1)
        score=1;
    else
        score=3;
    end
else
    if(valence_trial==1)
        score=2;
    else
        score=4;
    end
end
        

% --- Executes on button press in startButton.
function startButton_Callback(hObject, eventdata, handles)
% hObject    handle to startButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of startButton

set(handles.startButton,'Enable','off');
set(handles.stopButton,'Enable','on');
set(handles.stopButton,'Visible','on');
set(handles.pauseButton,'Enable','on');
set(handles.resumeButton,'Enable','off');
set(handles.pauseButton,'Visible','on');
set(handles.resumeButton','Visible','off');

do_acquisition(handles);


% --- Executes during object deletion, before destroying properties.
function lickingRatePlot_DeleteFcn(hObject, eventdata, handles)
% hObject    handle to lickingRatePlot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pauseButton.
function pauseButton_Callback(hObject, eventdata, handles)
% hObject    handle to pauseButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of pauseButton
global running_state, global s;
running_state=2;
fwrite(s,'P');
set(handles.resumeButton,'Visible','on');
set(handles.resumeButton,'Enable','on');
set(handles.pauseButton','Visible','off');
set(handles.pauseButton','Enable','off');


% --- Executes on button press in stopButton.
function stopButton_Callback(hObject, eventdata, handles)
% hObject    handle to stopButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of stopButton
global running_state, global s;
running_state=0;
fwrite(s,'S');

set(handles.stopButton,'Enable','off');
set(handles.startButton,'Enable','on');
set(handles.startButton,'Visible','on');
set(handles.pauseButton,'Enable','off');
set(handles.resumeButton,'Enable','off');
set(handles.pauseButton,'Visible','on');
set(handles.resumeButton','Visible','off');


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over stopButton.
function stopButton_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to stopButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in resumeButton.
function resumeButton_Callback(hObject, eventdata, handles)
% hObject    handle to resumeButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of resumeButton

global running_state, global s;
running_state=1;
fwrite(s,'R');
set(handles.pauseButton,'Visible','on');
set(handles.pauseButton,'Enable','on');
set(handles.resumeButton','Visible','off');
set(handles.resumeButton','Enable','off');


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over resumeButton.
function resumeButton_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to resumeButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over pauseButton.
function pauseButton_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to pauseButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
