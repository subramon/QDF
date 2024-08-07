#!/bin/bash
set -e 
cd ../../src
make ut_squeeze
export PATH=$PATH:$PWD
cd -
infile_a="../data/test_squeeze_1a.csv";
infile_b="../data/test_squeeze_1b.csv";
VG="valgrind --leak-check=full --show-leak-kinds=all "
$VG ut_squeeze $infile_a $infile_b 1>_x 2>&1
grep "definitely lost: 0 bytes in 0 blocks" _x 1>_y 2>&1
grep "SUCCESS" _x 1>_y 2>&1
#----------------------------
echo "Successfully completed $0 in $PWD"

