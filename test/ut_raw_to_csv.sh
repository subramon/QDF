#!/bin/bash
set -e 
infile=raw_to_csv_in_1.csv
outfile=raw_to_csv_out_1.csv
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes  "
VG=" "
$VG ut_raw_to_csv $infile _$outfile
diff $outfile _$outfile
#--------------------------------
echo "Success for $0 in $PWD"


