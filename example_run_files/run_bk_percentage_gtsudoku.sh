#!/bin/bash

for j in $(seq 0 0.1 0.9)
do

printf "\n\ngtsudoku Q1 init $j\n\n" >> results_quacq_gtsudoku

for i in $(seq 1 10)
do
./run quacq -solp -fs3 -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f gtsudoku
done

done

