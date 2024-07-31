#!/bin/bash
set -e
VG="valgrind --leak-check=full --show-leak-kinds=all "  
VG=" "
$VG ut_date_to_hol ../test/week_start_date_to_hols.txt _xx
diff _xx ../test/out_date_to_hols.txt
echo "Successfully completed $0 in $PWD"
