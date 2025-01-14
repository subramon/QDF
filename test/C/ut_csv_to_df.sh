#!/bin/bash
set -e 
cd ../../src
make ut_csv_to_df
export PATH=$PATH:$PWD
cd -
rm -f _*
infile="../data/in3.csv"
opfile="_out.csv"
cols="I1,I2,I4,I8,F4,F8,X8"
qtypes="I1,I2,I4,I8,F4,F8,F8"
VG="valgrind --leak-check=full --show-leak-kinds=all "
set +e
$VG ut_csv_to_df $infile $cols $qtypes $opfile 1>_x 2>&1
set -e
grep "SUCCESS" _x 1>/dev/null 2>&1
grep "definitely lost: 0 bytes in 0 blocks" _x 1>/dev/null 2>&1
#----------------------------
infile="../data/in2.csv"
cols="tm,i1"
qtypes="TM1:%Y-%m-%d,I1" 
$VG ut_csv_to_df $infile $cols $qtypes $opfile 1>_x 2>&1
grep "definitely lost: 0 bytes in 0 blocks"  _x 1>/dev/null 2>&1
#----------------------------
echo "Successfully completed $0 in $PWD"

