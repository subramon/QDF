#!/bin/bash
set -e
ispc -O3 --pic alt_if_a_then_not_b.ispc \
  -o alt_if_a_then_not_b.o -h alt_if_a_then_not_b.h
gcc main.c if_a_then_not_b.c alt_if_a_then_not_b.o
./a.out gcc
./a.out ispc
echo "Completed $0 in $PWD"
