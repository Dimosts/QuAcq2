#!/bin/bash

printf "\n\npurdey Q1\n\n" >> results_quacq_purdey

for i in $(seq 1 10)
do
./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f purdey
done

