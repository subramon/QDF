#!/bin/bash
set -e
if [ $# != 2 ]; then echo "Usage is $0 <dir> "; exit 1; fi
fnum=$1
DIR=$2
test -d $DIR
datafile=data${fnum}.csv
frmla=formula${fnum}.R
cols=col_names_${fnum}.csv
types=col_types_${fnum}.csv
while read -r line; do 
  test -f $DIR/$frmla 
  test -f $DIR/$cols 
  test -f $DIR/$types 
  test -f $DIR/$line
 echo ut3 $DIR/$frmla $DIR/$cols $DIR/$types $DIR/$line
  ut3 $DIR/$frmla $DIR/$cols $DIR/$types $DIR/$line
done < $DIR/$datafile 
echo "Completed $0 $1 $2 "
