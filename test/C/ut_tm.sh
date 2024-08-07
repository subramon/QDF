#!/bin/bash
set -e 
cd ../../src
make ut_tm
export PATH=$PATH:$PWD
cd -
#---------1-------------------
infile="../data/in4.csv"
cols="tm"
qtypes="TM1:%m/%d/%Y" 
opfile=_time_band.csv 
VG="valgrind --leak-check=full --show-leak-kinds=all "
$VG ut_tm $infile $cols $qtypes tm $opfile 1>_x 2>&1
grep "SUCCESS" _x 1>_y 2>&1
grep "definitely lost: 0 bytes in 0 blocks" _x 1>_y 2>&1
diff _time_band.csv ../data/time_band_1.csv 
#----------------------------
echo "Successfully completed $0 in $PWD"
