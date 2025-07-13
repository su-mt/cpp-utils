#!/usr/bin/env bash

FILE="file.txt"
WARMUP=1
RUNS=5
PREPARE="sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'"

echo "⏱️ Benchmarking external merge sort vs std::stable_sort on $FILE..."

hyperfine \
    --warmup $WARMUP \
    --runs $RUNS \
    --prepare "$PREPARE" \
    "./sort -m -s $FILE out_m_$FILE" \
    "./stdsort -s --stable $FILE out_stablesort_$FILE"
