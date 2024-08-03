#!/bin/bash
set -e 
cd ../../src
make ut_read_csv
export PATH=$PATH:$PWD
cd -
infile="../data/in3.csv"
cols="I1,I2,I4,I8,F4,F8"
qtypes="I1,I2,I4,I8,F4,F8"
VG="valgrind --leak-check=full --show-leak-kinds=all "
$VG ut_read_csv $infile $cols $qtypes 1>_x 2>&1
grep "definitely lost: 0 bytes in 0 blocks" _x 1>_y 2>&1
#----------------------------
infile="../data/in2.csv"
cols="tm,i1"
qtypes="TM1:%Y-%m-%d,I1" 
$VG ut_read_csv $infile $cols $qtypes 1>_x 2>&1
grep "definitely lost: 0 bytes in 0 blocks" _x 1>_y 2>&1
#----------------------------
echo "Successfully completed $0 in $PWD"

