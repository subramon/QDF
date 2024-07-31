#/bin/bash
set -e 
VG="valgrind --leak-check=full --show-leak-kinds=all "
VG=" "

dsid=tcin_10988565_dist_loc_i_1921_
dsid=tcin_10879977_dist_loc_i_1907_
dir="../data/"

data_csv="${dir}data_${dsid}.csv";     test -f $data_csv
data_rbc="${dir}data_${dsid}.rbc";     test -f $data_rbc
model_rbc="${dir}model_${dsid}.rbc";   test -f $model_rbc
# TODO model_json="${dir}model_${dsid}.json"; test -f $model_json
fcast_rbc="./_fcast_${dsid}.rbc";   
fcast_csv="./_fcast_${dsid}.csv";   

$VG ut_jni $data_csv csv $model_rbc rbc $fcast_csv csv

echo "Successfully completed $0 in $PWD"
