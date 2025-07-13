#!/usr/bin/env bash
# benchmark.sh


DATASETS=(
  "ints.txt"
  "floats.txt"
  "small.txt"
  "big.txt"
)


ALGORITHMS=(
  "-q -i"  # quick + int
  "-m -i"  # merge + int
  "-q -f"  # quick + float
  "-m -f"  # merge + float
)


PREPARE="sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'"


WARMUP=3


hyperfine \
  --warmup $WARMUP \
  --prepare "$PREPARE" \
  --export-csv benchmark_results.csv \
  \
  # ints.txt: quick vs merge
  "./sort -q -i ints.txt out_q_ints.txt" \
  "./sort -m -i ints.txt out_m_ints.txt" \
  \
  # floats.txt: quick vs merge
  "./sort -q -f floats.txt out_q_floats.txt" \
  "./sort -m -f floats.txt out_m_floats.txt" \
  \
  # small.txt: quick vs merge
  "./sort -q -s small.txt out_q_small.txt" \
  "./sort -m -s small.txt out_m_small.txt" \
  \
  # big.txt: quick vs merge
  "./sort -q -s big.txt out_q_big.txt" \
  "./sort -m -s big.txt out_m_big.txt"
