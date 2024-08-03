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
bash ut_read_csv.sh
echo "PREMATURE $0 in $PWD "
exit 1 

$VG ut_cells_to_vals
bash ut_concat.sh
$VG ut_config mgcv
bash ut_copy.sh
$VG ut_line_to_cells
$VG ut_logical_op
$VG ut_rbc_makers
$VG ut_mixed
$VG ut_pr
bash ut_raw_to_csv.sh
$VG ut_rbc_named_vec
bash ut_tcin_to_cluster.sh
$VG ut_squeeze
bash ut_tm.sh
$VG ut_unique
#-- tests for mtmgcv
echo "PREMATURE"; exit 0 # TODO P3 Need to fix or delete following
bash ut_do_R.sh
bash ut_plp1.sh
bash ut_plp2.sh

TODO bash ut_merge_df.sh 
echo "Completed $0 in $PWD successfully"
