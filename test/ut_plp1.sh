#!/bin/bash
# TODO Confirm out files with Mahdi
# To prepare an input file, select a data set from a data dump
# charsub <filename> 1 44 # where charsub is from utils/src/
# cut -f 6,10,12,35 -d "," <filename> > <csvfilename>
# We extract co_loc_i,holiday_array,sls_unit_q,week_start_date
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes "
VG=""
set -e 
configs=mgcv
infile="../data/ut_plp1_in1.csv"
cols="co_loc_i,sls_unit_q,week_start_date"
qtypes="I2,F4,TM:%Y-%m-%d"
outfile="../data/ut_plp1_out1.csv"
make -C ../src/ ut_plp1 
rm -f _xx
$VG ut_plp1 $configs $infile $cols $qtypes _xx
diff _xx $outfile 1>/dev/null 2>&1
if [ $? != 0 ]; then echo "ERR: diff _xx, $outfile"; exit 1; fi
echo "Test $0 completed successfully"

