#!/bin/bash
################
## Run RCSS2D matches in a remote host to collect match logs.
## 
##  Teams should follow the standard of RoboCup competitions but be placed inside a ~/Public/ folder.
##  A "startrcssteam.sh" script should be put in the HOME folder in the remote host and be responsible for  
##      starting a team given its directory location.
##
## Arg 1: Number of matches to run
## Arg 2: Team placed on the left side (called first)
## Arg 3: Team placed on the right side (called second)
## Arg 4: Hostname for remote server where matches are run. Set the hostname and SSH options in ~/.ssh/config
################

NUM_MATCHES=$1
TEAML=$2
TEAMR=$3
REMOTE_HOST=$4

for ((i=1; i<=NUM_MATCHES; i++)) do
    timestamp=`date +%Y%m%d-%T`;
    rcssserver \
        server::game_log_compression=6 \
        server::nr_extra_halfs=0 \
        server::text_log_compression=6 \
        server::auto_mode=on \
        server::fullstate_l=on \
        server::fullstate_r=on \
        server::penalty_shoot_outs=off \
        server::synch_mode=on 2>${timestamp}_rcssserver.err &
    sleep 2; ## Wait a bit for simulator to start
    ssh $REMOTE_HOST "cd \$HOME; ./startrcssteam.sh Public/$TEAML" 1>${timestamp}_${TEAML}_${i}.out 2>${timestamp}_${TEAML}_${i}.err & # Background
    ssh $REMOTE_HOST "cd \$HOME; ./startrcssteam.sh Public/$TEAMR;" 1>${timestamp}_${TEAMR}_${i}.out 2>${timestamp}_${TEAMR}_${i}.err; # Wait for it to finish
    sleep 2; ## Wait a bit before calling next match, in case the 1st team is still finishing
done
