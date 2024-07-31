configs=mgcv
infile="step_by_step/CASE1/d3.csv"
cols="sls_unit_q,sls_unit_q_l1,sls_unit_q_l2,sls_unit_q_l3,sls_unit_q_l4,sls_unit_q_l5,co_loc_i,t_o_y,time_band,columbusday,goodfriday,easter,fathersday_minus,fathersday,laborday_minus,laborday,mardigras,memorialday_minus,memorialday,mlkday,mothersday_minus,mothersday,presidentsday,superbowl_minus,superbowl,thanksgiving_minus,thanksgiving,cyber_monday,valentines_minus,valentines,stpatricks,cincodemayo,julyfourth_minus,julyfourth,halloween_minus,halloween,veteransday,christmas_minus,christmas,newyearsday_minus,newyearsday,nn_sls_unit_q_l1,nn_sls_unit_q_l2,nn_sls_unit_q_l3,nn_sls_unit_q_l4,nn_sls_unit_q_l5"
qtypes="F8,F8,F8,F8,F8,F8,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I4,I1,I1,I1,I1,I1"
frmlas_file="ut_chk_against_R_frmlas1.csv"
keys_file="ut_chk_against_R_keys1.csv"
_out_prefix="_xx"
_frmlas_R_file="_yy"
cat $frmlas_file | sed s'/^frmla.*sls_unit_q/sls_unit_q/'g > $_frmlas_R_file
ut_do_R $configs $infile $cols $qtypes $keys_file $frmlas_file _$out_prefix

./run_gam.R --input="$infile" --formula="$_frmlas_R_file"
echo "Complted $0 in $PWD successfully"
