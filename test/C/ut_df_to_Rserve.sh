#!/bin/bash
set -e 
pushd .
cd ../../src
make ut_df_to_Rserve
export PATH=$PATH:$PWD
popd

server="localhost"
port=6311
infile="../data/test_df_to_Rserve.csv"
cols="i1,i2,i4,_nn_i4,xi4,f4,f8,tm1"
qtypes="I1,I2,I4,BL,I4,F4,F8,TM1:%Y-%m-%d"
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes  "
$VG ut_df_to_Rserve $server $port $infile $cols $qtypes $n 1>_x 2>&1
grep "SUCCESS" _x 1>_y 2>&1
grep "definitely lost: 0 bytes in 0 blocks" _x 1>_y 2>&1
echo "Successfully completed $0 in $PWD"
