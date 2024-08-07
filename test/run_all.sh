#!/bin/bash
VG="valgrind --leak-check=full --show-leak-kinds=all " 
VG=" "
set -e
pushd .
cd ..
source to_source
cd src/
make clean && make 
popd 

cd C/
bash ut_concat.sh
bash ut_copy.sh
bash ut_logical_op.sh
bash ut_mixed_array.sh
bash ut_named_vec.sh
bash ut_pr.sh
bash ut_qdf_makers.sh
bash ut_read_csv.sh
bash ut_squeeze.sh
bash ut_tm.sh
bash ut_uniques.sh

echo "Completed $0 in $PWD successfully"
