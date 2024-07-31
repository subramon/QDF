#!/bin/bash
set -e 
cd ../src
make ut_concat
cd -
infile="../test/in6.csv"
cols="i1,i2,i4,i8,f4,f8,tm,hl"
qtypes="I1,I2,I4,I8,F4,F8,TM:%Y-%m-%d,HL"
n=16 # number of times we concat same RBC
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes  "
VG=" "
$VG ut_concat $infile $cols $qtypes $n
#----------------------------
echo "Successfully completed $0 in $PWD"

