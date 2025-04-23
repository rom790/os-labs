#!/bin/bash

# Пути к исполняемым файлам
EXECUTABLES=("src/5/5.4/5.4.4/5.4.4-1" "src/5/5.4/5.4.4/5.4.4-2" "src/5/5.4/5.4.4/5.4.4-3")

SAME_PRIORITY=10
# Функция для ожидания завершения группы процессов
wait_for_processes() {
    for pid in "$@"; do
        wait "$pid"
    done
}

echo "Запуск тестов с sched_yeld..."
pids=()
for exe in "${EXECUTABLES[@]}"; do
    echo "Запуск $exe с приоритетом $SAME_PRIORITY"
    sudo taskset -c 0 "$exe" "$SAME_PRIORITY" &
    pids+=($!)
done
wait_for_processes "${pids[@]}"




