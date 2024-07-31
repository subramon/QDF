#!/bin/bash
set -e
../src/ut_tcin_to_cluster ../data/tcin_map_1.rbc 13020067 1>_x
grep FOUND _x
set +e
../src/ut_tcin_to_cluster ../data/tcin_map_1.rbc 1234 1>_x 2>&1
status=$?
if [ $status == 0 ]; then echo "ERROR"; exit 1; fi 

echo "$0 in $PWD completed successfully"
