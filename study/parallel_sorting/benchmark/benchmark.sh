#!/usr/bin/env bash

# Файлы для тестирования
FILES=("ints.txt" "floats.txt" "small.txt" "big.txt")

# Удаление предыдущих результатов
rm -f benchmark_results.csv

# Сброс кэша (требует sudo)
PREPARE="sudo sh -c 'echo 3 > /proc/sys/vm/drop_caches'"

# Прогрев и количество прогонов
WARMUP=3
RUNS=10

for file in "${FILES[@]}"; do
    echo "⏱️ Benchmarking $file..."

    # Определение типа
    type_flag="-i"
    [[ "$file" == "floats.txt" ]] && type_flag="-f"
    [[ "$file" == "small.txt" || "$file" == "big.txt" ]] && type_flag="-s"

    # Создание набора команд
    COMMANDS=()
    COMMANDS+=("./sort -m $type_flag $file out_m_$file")

    if [[ "$file" != "big.txt" ]]; then
        # std::sort и std::stable_sort (файл бинарника — stdsort)
        COMMANDS+=("./sort -q $type_flag $file out_q_$file")
        COMMANDS+=("./stdsort $type_flag $file out_sort_$file")
        COMMANDS+=("./stdsort $type_flag --stable $file out_stablesort_$file")
        THIS_WARMUP=$WARMUP
        THIS_RUNS=$RUNS
    else
        THIS_WARMUP=0
        THIS_RUNS=1
    fi

    # Запуск hyperfine
    hyperfine \
        --warmup "$THIS_WARMUP" \
        --runs "$THIS_RUNS" \
        --prepare "$PREPARE" \
        --export-csv benchmark_${file%.txt}.csv \
        "${COMMANDS[@]}"
done

# Слияние CSV-файлов (если установлен `csvstack`)
if command -v csvstack &> /dev/null; then
    echo " Объединение CSV-файлов в benchmark_results.csv"
    csvstack -g ints,floats,small,big -n file benchmark_*.csv > benchmark_results.csv
