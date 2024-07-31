# This test harness starts from the Hive output and ends with the
# (1) post reading data in outfile1
# (2) post plp1 data in outfile2
# outfile2 contains the holiday columns but those are all 0
# They are set in plp2
# outfile2 contains the lag columns set correctly 
#/bin/bash
set -e
config="mgcv"
infile="../test/step_by_step/CASE1/d0.txt"
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes "

outfile1="_xx1"
outfile2="_xx2"
goodfile1="../test/step_by_step/CASE1/d3.csv"
goodfile2="../test/step_by_step/CASE1/d4.csv"
mkdir -p /tmp/mgcv_in/ # needs to be done by read_configs
$VG ut_read_hive $config $infile $outfile1 $outfile2
set +e
diff $outfile1 $goodfile1 1>/dev/null 2>&1
if  [ $? != 0 ]; then echo "ERROR: difference 1 observed"; fi
diff $outfile2 $goodfile2 1>/dev/null 2>&1
if  [ $? != 0 ]; then echo "ERROR: difference 2  observed"; fi
echo "Completed $0 in $PWD successfully"
