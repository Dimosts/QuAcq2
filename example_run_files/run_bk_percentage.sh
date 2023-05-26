#!/bin/bash

for j in $(seq 0.7 0.1 0.7)
do

#printf "nothing"
printf "\n\nrandom122 Q1 init $j FS1\n\n" >> results_quacq_random122

for i in $(seq 1 10)
do
./run quacq -solp -fs -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f random122
done

printf "\n\nrandom122 Q1 init $j\n\n" >> results_quacq_random122

for i in $(seq 1 10)
do
./run quacq -solp -fs3 -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f random122
done

#printf "\n\npurdey Q1 init $j\n\n" >> results_quacq_purdey

#for i in $(seq 1 10)
#do
#./run quacq -solp -fs3 -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f purdey
#done

#printf "\n\nsudoku Q1 init $j\n\n" >> results_quacq_sudoku

#for i in $(seq 1 10)
#do
#./run quacq -solp -fs3 -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f sudoku
#done

#printf "\n\njsudoku Q1 init $j\n\n" >> results_quacq_jsudoku

#for i in $(seq 1 10)
#do
#./run quacq -solp -fs3 -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f jsudoku
#done

#printf "\n\nlatin Q1 init $j\n\n" >> results_quacq_qcp-10-67-0

#for i in $(seq 1 10)
#do
#./run quacq -solp -fs3 -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f qcp-10-67-0
#done

#printf "\n\nzebra Q1 init $j\n\n" >> results_quacq_zebra

#for i in $(seq 1 10)
#do
#./run quacq -solp -fs3 -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f zebra
#done

#printf "\n\nrlfap Q1 init $j\n\n" >> results_quacq_rlfap

#for i in $(seq 1 10)
#do
#./run quacq -solp -fs3 -fc 1 -ge -bp $j -nc -nu -h 3 -domh 1 -f rlfap
#done

done

