#!/bin/bash
set -e 
cd ../src
make ut_copy
cd -
#----------------------------
infile="../test/in5.csv"
cols="x,y"
qtypes="F8,I4"
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"
VG=" "
$VG ut_copy $infile $cols $qtypes "x" "y"
#----------------------------

echo "Successfully completed $0 in $PWD"

