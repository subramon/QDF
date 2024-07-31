#!/bin/bash
set -e 
cd ../src
make ut_tm
cd -
#----------------------------
infile="../test/in4.csv"
cols="tm"
qtypes="TM:%m/%d/%Y" 
VG="valgrind --leak-check=full --show-leak-kinds=all "
VG=""
$VG ut_tm $infile $cols $qtypes tm
diff ../test/_time_band.csv ../test/time_band_1.csv 
#----------------------------

echo "Successfully completed $0 in $PWD"

