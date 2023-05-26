#!/bin/bash


for j in $(seq 0 0.1 0.9)
do

printf "\n\ngolomb_8 Q1 init $j\n\n" >> results_quacq_golomb_8

for i in $(seq 1 10)
do
./run quacq -solp -fs3 -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f golomb_8
done

done
