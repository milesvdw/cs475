#!/usr/bin/env bash

echo "Performance Data"
echo "Padding,1 Thread,2 Threads,3 Threads"

for i in `seq 0 16`; do
g++ -DNUM=${i} -fopenmp main.c -o main
./main
done
