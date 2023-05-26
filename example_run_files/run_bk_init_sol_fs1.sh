
#!/bin/bash

printf "\n\nrlfap Q0 init cl FS1 \n\n" >> results_quacq_rlfap

for i in $(seq 1 10)
do
./run quacq -sol -fs -fc 1 -nc -nu -h 2 -domh 1 -f rlfap -i
done

printf "\n\npurdey Q0 init cl FS1\n\n" >> results_quacq_purdey

for i in $(seq 1 10)
do
./run quacq -sol -fs -fc 1 -nc -nu -h 2 -domh 1 -f purdey -i
done

printf "\n\nzebra Q0 init cl FS1\n\n" >> results_quacq_zebra

for i in $(seq 1 10)
do
./run quacq -sol -fs -fc 1 -nc -nu -h 2 -domh 1 -f zebra -i
done

printf "\n\ngolomb_8 Q0 init cl FS1 \n\n" >> results_quacq_golomb_8

for i in $(seq 1 15)
do
./run quacq -sol -fs -fc 1 -nc -nu -h 2 -domh 1 -f golomb_8 -i
done

printf "\n\njsudoku Q0 init cl FS1 \n\n" >> results_quacq_jsudoku

for i in $(seq 1 10)
do
./run quacq -sol -fs -fc 1 -nc -nu -h 2 -domh 1 -f jsudoku -i
done


