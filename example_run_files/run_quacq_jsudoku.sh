#!/bin/bash

printf "\n\njsudoku sol\n\n" >> results_quacq_jsudoku

./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f jsudoku

