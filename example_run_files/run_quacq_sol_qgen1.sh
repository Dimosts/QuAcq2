#!/bin/bash

printf "\n\nrandom122 q0\n\n" >> results_quacq_random122

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f random122
done

printf "\n\nrandom12 q0\n\n" >> results_quacq_random12

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f random12
done

printf "\n\npurdey q0\n\n" >> results_quacq_purdey

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f purdey
done

printf "\n\nrlfap q0\n\n" >> results_quacq_rlfap

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f rlfap
done

printf "\n\nzebra q0\n\n" >> results_quacq_zebra

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f zebra
done

printf "\n\nsudoku q0\n\n" >> results_quacq_sudoku

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f sudoku
done

printf "\n\njsudoku q0\n\n" >> results_quacq_jsudoku

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f jsudoku
done

printf "\n\nlatin q0\n\n" >> results_quacq_qcp-10-67-0

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f qcp-10-67-0
done

printf "\n\ngolomb_8 q0\n\n" >> results_quacq_golomb_8

for i in $(seq 1 10)
do
./run quacq -sol -fs3 -fc 1 -nc -nu -h 2 -domh 1 -f golomb_8
done

