#!/bin/bash

set -e

RUNS=100  # сколько параллельных запусков

run_one() {
    i=$1
    LOG_FILE="crossroads_${i}.log"


    # запуск программы в фоне
    ../a.out "$LOG_FILE" 1>/dev/null &
    PID=$!

    # ждем 40 секунд
    sleep 100

    # отправляем SIGINT
    kill -SIGINT $PID

    # ждем завершения процесса
    wait $PID


    python3 ../check.py "$LOG_FILE"

    rm "$LOG_FILE"
}

export -f run_one

# параллельный запуск
parallel -j $RUNS run_one ::: $(seq 1 $RUNS)