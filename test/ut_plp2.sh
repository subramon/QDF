#!/bin/bash
# TODO Confirm out files with Mahdi
VG=""
VG="valgrind --leak-check=full --show-leak-kinds=all"
set -e 
configs=mgcv
infile=../data/ut_plp2_in1.csv
test -f $infile
cols="sls_unit_q,sls_unit_q_l1,sls_unit_q_l2,sls_unit_q_l3,sls_unit_q_l4,sls_unit_q_l5,co_loc_i,t_o_y,time_band,holiday_bmask,columbusday,goodfriday,easter,fathersday_minus,fathersday,laborday_minus,laborday,mardigras,memorialday_minus,memorialday,mlkday,mothersday_minus,mothersday,presidentsday,superbowl_minus,superbowl,thanksgiving_minus,thanksgiving,cyber_monday,valentines_minus,valentines,stpatricks,cincodemayo,julyfourth_minus,julyfourth,halloween_minus,halloween,veteransday,christmas_minus,christmas,newyearsday_minus,newyearsday,nn_sls_unit_q_l1,nn_sls_unit_q_l2,nn_sls_unit_q_l3,nn_sls_unit_q_l4,nn_sls_unit_q_l5"
qtypes="F8,F8,F8,F8,F8,F8" # for sls_unit_q, slq_unit_q_l1, ...
qtypes="$qtypes,I4,I4,I4"  # for co_loc_i, t_oy, time_band
qtypes="$qtypes,HL"  # for holiday_bmask
qtypes="$qtypes,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4" # for holidays
qtypes="$qtypes,I1,I1,I1,I1,I1" # for nn_slq_unit_q_l1, ...
out_prefix="ut_plp2_out1"
make -C ../src/ ut_plp2
$VG ut_plp2 $configs $infile $cols $qtypes "$out_prefix"
echo "PREMATURE: diff to be  checked"
exit 0

diff _${out_prefix}_f1.csv ../data/${out_prefix}_f1.csv 
diff _${out_prefix}_f2.csv ../data/${out_prefix}_f2.csv 
diff _${out_prefix}_f3.csv ../data/${out_prefix}_f3.csv 
diff _${out_prefix}_f4.csv ../data/${out_prefix}_f4.csv 
diff _${out_prefix}_f5.csv ../data/${out_prefix}_f5.csv 

echo "Test $0 completed successfully"

