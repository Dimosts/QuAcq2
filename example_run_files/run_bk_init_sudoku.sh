#!/bin/bash

#printf "\n\nsudoku Q1 init cl\n\n" >> results_quacq_sudoku

#for i in $(seq 1 10)
#do
#./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f sudoku -i
#done


printf "\n\nsudoku Q0 init cl\n\n" >> results_quacq_sudoku

for i in $(seq 1 3)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f sudoku -i
done

