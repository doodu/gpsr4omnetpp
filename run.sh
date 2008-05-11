#!/bin/bash

echo $1:$2:$3:$4">" >> omnetpp.data

./create_ini.py $1 $2 $3 $4
./802.11
cat omnetpp.vec | grep "^[0-9]" | ./data_process.py >> omnetpp.data