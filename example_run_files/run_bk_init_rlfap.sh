#!/bin/bash

printf "\n\nrlfap Q1 init cl\n\n" >> results_quacq_rlfap

for i in $(seq 1 10)
do
./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f rlfap -i
done


printf "\n\nrlfap Q0 init cl\n\n" >> results_quacq_rlfap

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f rlfap -i
done

