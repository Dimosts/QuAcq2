#!/bin/bash

printf "\n\nrandom122 sol qgen\n\n" >> results_quacq_random122

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f random122
done

