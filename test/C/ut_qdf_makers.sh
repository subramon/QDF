#!/bin/bash
set -e 
cd ../../src
make ut_qdf_makers
export PATH=$PATH:$PWD
cd -
VG="valgrind --leak-check=full --show-leak-kinds=all "
$VG ut_qdf_makers 1>_x 2>&1
grep "SUCCESS" _x 1>_y 2>&1
grep "definitely lost: 0 bytes in 0 blocks" _x 1>_y 2>&1
#----------------------------
echo "Successfully completed $0 in $PWD"

