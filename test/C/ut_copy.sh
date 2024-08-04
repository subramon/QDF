#!/bin/bash
set -e 
cd ../../src
make ut_copy
export PATH=$PATH:$PWD
cd -
#----------------------------
infile="../data/in5.csv"
cols="x,y"
qtypes="F8,I4"
VG=" "
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"
$VG ut_copy $infile $cols $qtypes "x" "y" 1>_x 2>&1
grep "SUCCESS" _x 1>_y 2>&1
grep "definitely lost: 0 bytes in 0 blocks" _x 1>_y 2>&1
#----------------------------

echo "Successfully completed $0 in $PWD"

