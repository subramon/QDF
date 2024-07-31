#!/bin/bash
set -e 
infile=../test/step_by_step/CASE2/raw.csv
outfile=../test/step_by_step/CASE2/in_for_plp1.csv
ut_raw_to_csv $infile $outfile
#--------------------------------
infile=../test/step_by_step/CASE2/in_for_plp1.csv
outfile=../test/step_by_step/CASE2/in_for_plp2.csv
configs=mgcv
cols="co_loc_i,holiday_array,sls_unit_q,week_start_date"
qtypes="I2,HL,F4,TM:%Y-%m-%d"
ut_plp1 $configs $infile $cols $qtypes $outfile 
#--------------------------------
infile=../test/step_by_step/CASE2/in_for_plp2.csv
outfile=../test/step_by_step/CASE2/in_for_mgcv.csv
cols="sls_unit_q,sls_unit_q_l1,sls_unit_q_l2,sls_unit_q_l3,sls_unit_q_l4,sls_unit_q_l5,co_loc_i,t_o_y,time_band,holiday_bmask,columbusday,goodfriday,easter,fathersday_minus,fathersday,laborday_minus,laborday,mardigras,memorialday_minus,memorialday,mlkday,mothersday_minus,mothersday,presidentsday,superbowl_minus,superbowl,thanksgiving_minus,thanksgiving,cyber_monday,valentines_minus,valentines,stpatricks,cincodemayo,julyfourth_minus,julyfourth,halloween_minus,halloween,veteransday,christmas_minus,christmas,newyearsday_minus,newyearsday,nn_sls_unit_q_l1,nn_sls_unit_q_l2,nn_sls_unit_q_l3,nn_sls_unit_q_l4,nn_sls_unit_q_l5"
qtypes="F4,F4,F4,F4,F4,F4" # for sls_unit_q, slq_unit_q_l1, ...
qtypes="$qtypes,I2,I2,I1"  # for co_loc_i, t_oy, time_band
qtypes="$qtypes,HL"  # for holiday_bmask
qtypes="$qtypes,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1,I1" # for holidays
qtypes="$qtypes,I1,I1,I1,I1,I1" # for nn_slq_unit_q_l1, ...
out_prefix="_xx"

rm -f ${out_prefix}* 
ut_plp2 $configs $infile $cols $qtypes "$out_prefix"
mv ${out_prefix}* ./step_by_step/CASE2/

echo "Success for $0 in $PWD"


