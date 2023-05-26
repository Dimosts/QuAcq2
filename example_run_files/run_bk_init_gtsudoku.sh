#!/bin/bash

printf "\n\ngtsudoku Q1 init cl\n\n" >> results_quacq_gtsudoku

for i in $(seq 1 5)
do
./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f gtsudoku -i
done


printf "\n\ngtsudoku Q0 init cl\n\n" >> results_quacq_gtsudoku

for i in $(seq 1 5)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f gtsudoku -i
done

