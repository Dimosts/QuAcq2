#!/bin/bash

printf "\n\njsudoku Q1 init cl\n\n" >> results_quacq_jsudoku

for i in $(seq 1 5)
do
./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f jsudoku -i
done


printf "\n\njsudoku Q0 init cl\n\n" >> results_quacq_jsudoku

for i in $(seq 1 5)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f jsudoku -i
done

