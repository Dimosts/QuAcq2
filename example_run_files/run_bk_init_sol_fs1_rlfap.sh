#!/bin/bash

printf "\n\nrlfap Q0 init cl FS1 \n\n" >> results_quacq_rlfap

for i in $(seq 1 20)
do
./run quacq -sol -fs -fc 1 -nc -nu -h 2 -domh 1 -f rlfap -i
done
