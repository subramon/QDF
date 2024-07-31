#!/bin/bash
if [ $# != 2 ]; then echo error; exit 1; fi
infile=$1
destdir=$2
test -f $infile
mkdir -p $destdir
colidx=1
ncols=40
while [ $colidx -le 40 ]; do
  cut -f $colidx -d "," $infile > _x
  min=`sort -n _x | head -2`
  max=`sort -n _x | tail -n 2`
  echo $colidx,$min,$max
  outfile=`head -1 _x | sed s'/"//'g `
  outfile=${outfile}.csv
  mv _x $destdir/$outfile
  colidx=`expr $colidx + 1`
done
echo "Completed $0 in $PWD successfully"
