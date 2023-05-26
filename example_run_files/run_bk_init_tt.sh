#!/bin/bash

printf "\n\ntt Q1 init cl\n\n" >> results_quacq_timetabling24

for i in $(seq 1 3)
do
./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f timetabling24 -i
done

printf "\n\ntt Q0 init cl\n\n" >> results_quacq_timetabling24

for i in $(seq 1 5)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f timetabling24 -i
done


