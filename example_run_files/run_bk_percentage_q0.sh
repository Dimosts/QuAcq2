#!/bin/bash

for j in $(seq 0.7 0.1 0.7)
do

printf "\n\nrandom122 Q0 fs3 init $j\n\n" >> results_quacq_random122

for i in $(seq 1 20)
do
./run quacq -sol -fs3 -fc 1 -bp $j -nc -nu -h 2 -domh 1 -f random122
done

done

