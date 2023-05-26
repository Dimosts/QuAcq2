#!/bin/bash

printf "\n\nsudoku sol\n\n" >> results_quacq_sudoku

./run quacq -solp -fs3 -fc 1 -ge -nc -nu -h 3 -domh 1 -f jsudoku
