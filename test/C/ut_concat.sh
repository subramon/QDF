#!/bin/bash
set -e 
pushd .
cd ../../src
make ut_concat
export PATH=$PATH:$PWD
popd

infile="../data/in6.csv"
cols="i1,i2,i4,i8,f4,f8,tm1"
qtypes="I1,I2,I4,I8,F4,F8,TM1:%Y-%m-%d"
n=16 # number of times we concat same RBC
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes  "
$VG ut_concat $infile $cols $qtypes $n 1>_x 2>&1
grep "SUCCESS" _x 1>_y 2>&1
grep "definitely lost: 0 bytes in 0 blocks" _x 1>_y 2>&1
#----------------------------
echo "Successfully completed $0 in $PWD"


