#!/bin/bash

printf "\n\npurdey Q1 init cl\n\n" >> results_quacq_purdey

for i in $(seq 1 10)
do
./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f purdey -i
done

printf "\n\nzebra Q1 init cl\n\n" >> results_quacq_zebra

for i in $(seq 1 10)
do
./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f zebra -i
done

#printf "\n\ngolomb_8 Q1 init cl\n\n" >> results_quacq_golomb_8

#for i in $(seq 1 15)
#do
#./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f golomb_8 -i
#done


