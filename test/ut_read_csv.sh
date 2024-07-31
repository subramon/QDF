#!/bin/bash
set -e 
cd ../src
make ut_read_csv
cd -
infile="../test/in3.csv"
cols="I1,I2,I4,I8,F4,F8"
qtypes="I1,I2,I4,I8,F4,F8"
VG="valgrind --leak-check=full --show-leak-kinds=all "
VG=" "
$VG ut_read_csv $infile $cols $qtypes
#----------------------------
infile="../data/in2.csv"
cols="tm,i1"
qtypes="TM:%Y-%m-%d,I1" 
$VG ut_read_csv $infile $cols $qtypes
#----------------------------

echo "Successfully completed $0 in $PWD"

