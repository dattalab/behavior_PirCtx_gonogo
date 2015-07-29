#!/usr/bin/env python
# -*- coding: utf-8 -*-
import xml.etree.ElementTree as ET
import sys
import json
import urllib
import random
sys.path.insert(0, '/usr/lib/python2.7/bridge/')

from bridgeclient import BridgeClient as bridgeclient

value = bridgeclient()

tree = ET.parse(urllib.urlopen('http://192.168.20.14/yun_config/' + str(sys.argv[1]) + '.xml'))

odors_tree=tree.findall('./wheel-set/wheel')
o=0
odors=[[] for _ in range(0,len(odors_tree))]
for r in odors_tree:
    odors[o] = [r.get('wheel_name'),int(r.get('wheel_valve'))]
    o = o+1
nb_odors = o
del o
del odors_tree

step_tree=tree.findall('./step[@step-id=\'' + str(sys.argv[2]) + '\']')[0]
basic_settings_tree=step_tree.find("experiment-settings/basic_settings")
settings=[int(basic_settings_tree.get("mode")),int(basic_settings_tree.get("nb_blocks")),int(basic_settings_tree.get("default_nb_trials_per_odor"))]
del basic_settings_tree
i=0
for a in settings:
    value.put('basic_settings/' + str(i),str(a))
    i=i+1
default_flows_tree=step_tree.find("experiment-settings/default_flows")
default_flows=[int(default_flows_tree.get("carrier_flow")),int(default_flows_tree.get("odor1_flow"))]
i=0
for a in default_flows:
    value.put('default_flows/' + str(i),str(a))
    i=i+1
del default_flows_tree
duration_scheme_tree=step_tree.find("experiment-settings/durations")
duration_scheme=[int(duration_scheme_tree.get("odor_sampling")),int(duration_scheme_tree.get("isi"))]
del duration_scheme_tree
i=0
for a in duration_scheme:
    value.put('duration_scheme/' + str(i),str(a))
    i=i+1

total_nb_trials=0
stimulus_set_tree=step_tree.findall('stimulus-set/stimulus-set-component')
stimulus_set = [[] for _ in range(0,len(stimulus_set_tree))]
i=0
for s in stimulus_set_tree:
    stimulus_set[i]=[[] for _ in range(0,6)]
    stimulus_set[i][0]=s.get('stimulus_name')
    if(stimulus_set[i][0] == None):
        find_odor_tree=tree.findall('./wheel-set/wheel[@wheel_valve=\'' + s.get('wheel_valve') + '\']')[0]
        stimulus_set[i][0]=find_odor_tree.get('wheel_name')
    value.put('s/' + str(i) + '/0',str(stimulus_set[i][0]))
    stimulus_set[i][1]=int(s.get('carrier_flow'))
    value.put('s/' + str(i) + '/1',str(stimulus_set[i][1]))
    stimulus_set[i][2]=int(s.get('wheel_valve'))
    value.put('s/' + str(i) + '/2',str(stimulus_set[i][2]))
    stimulus_set[i][3]=int(s.get('odor_flow'))
    value.put('s/' + str(i) + '/3',str(stimulus_set[i][3]))
#stimulus_set[i][4]=int(s.get('valence_code'))
#value.put('s/' + str(i) + '/4',str(stimulus_set[i][4]))
    if(s.get('nb_trials') == None):
        stimulus_set[i][5]=int(settings[2])
    else:
        stimulus_set[i][5]=int(s.get('nb_trials'))
    value.put('s/' + str(i) + '/5',str(stimulus_set[i][5]))
    total_nb_trials+=stimulus_set[i][5]
    i=i+1
nb_stimuli=i
value.put('nb_stimuli',str(nb_stimuli))
del i
del stimulus_set_tree

random.seed()

nb_generated=4*nb_stimuli*settings[2]
seedsequence=[random.randint(0,nb_stimuli-1) for _ in range(nb_generated+1)]
c_trials_per_odor=[0]*nb_stimuli
random_sequence=[]
random_sequence.append(seedsequence[0])
c_trials_per_odor[random_sequence[0]]=1
go=0
seq_id=0
consec_nb=0
max_consecutive=2
for i in range(1,total_nb_trials):
    if(go==0):
        seq_id=seq_id+1
        if(seq_id>nb_generated):
            go=i
            consec_nb=max_consecutive+1
            while((go == 0) and ((consec_nb > max_consecutive) or ((c_trials_per_odor[seedsequence[seq_id]]+1) > stimulus_set[seedsequence[seq_id]][5]))):
                seq_id=seq_id+1
                if(seq_id > nb_generated):
                    go=i
                    consec_nb=max_consecutive+1
                if(random_sequence[i-1] != seedsequence[seq_id]):
                        consec_nb=0
        if(go == 0):
            random_sequence.append(seedsequence[seq_id])
            c_trials_per_odor[seedsequence[seq_id]]+=1
            if(random_sequence[i-1] == random_sequence[i]):
                consec_nb=consec_nb+1
            else:
                consec_nb=0

if(go != 0):
    resting_odors=[]
    for o in range(nb_stimuli):
        if(c_trials_per_odor[o] < stimulus_set[o][5]):
            for i in range(c_trials_per_odor[o],stimulus_set[o][5]+1):
                resting_odors.append(o)
    random.shuffle(resting_odors)
    for i in range(v):
        random_sequence[go+i]=resting_odors[i]

for i,random_stimulus in enumerate(random_sequence):
    value.put('rs/' + str(i), str(random_stimulus))