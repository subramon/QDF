#!/bin/bash
# TODO 
set -e
infiles="./data/merge11.bin:./data/merge12.bin:./data/merge13.bin:./data/merge14.bin:./data/merge15.bin"
cols="sls_unit_q,co_loc_i,baseprice_lift,promo_lift,promo_price_lift,columbusday,goodfriday,easter,fathersday.minus,fathersday,laborday.minus,laborday,mardigras,memorialday.minus,memorialday,mlkday,mothersday.minus,mothersday,presidentsday,superbowl.minus,superbowl,thanksgiving.minus,thanksgiving,cyber.monday,valentines.minus,valentines,stpatricks,cincodemayo,julyfourth.minus,julyfourth,halloween.minus,halloween,veteransday,christmas.minus,christmas,newyearsday.minus,newyearsday,t_o_y,n_week,n_ind,some_date,holidays"
outfile="./data/_merged_df.csv"
expected_outfile="./data/merged_df.csv"

luajit mk_rbc_for_test_merge.lua
VG=""
VG=="valgrind --leak-check=full --show-leak-kinds=all "
$VG ut_merge_df $infiles $cols $outfile 
diff $outfile $expected_outfile
echo "Successfully completed $0 in $PWD"
