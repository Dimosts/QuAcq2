#!/bin/bash


printf "\n\nzebra solp ge\n\n" >> results_quacq_zebra

for i in $(seq 1 10)
do
./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f zebra
done

printf "\n\nzebra q0\n\n" >> results_quacq_zebra

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f zebra
done

