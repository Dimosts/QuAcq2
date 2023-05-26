#!/bin/bash

printf "\n\ngolomb_8 Q1 init cl\n\n" >> results_quacq_golomb_8

for i in $(seq 1 10)
do
./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f golomb_8 -i
done


printf "\n\ngolomb_8 Q0 init cl\n\n" >> results_quacq_golomb_8

for i in $(seq 1 5)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f golomb_8 -i
done

