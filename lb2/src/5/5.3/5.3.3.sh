#!/bin/bash

# Пути к исполняемым файлам
EXECUTABLES=("src/5/5.3/5.3-1" "src/5/5.3/5.3-2" "src/5/5.3/5.3-3")



# Приоритеты для запуска с разными приоритетами (для каждого из 3-х процессов)
DIFFERENT_PRIORITIES=(52 14 94)

# Функция для ожидания завершения группы процессов
wait_for_processes() {
    for pid in "$@"; do
        wait "$pid"
    done
}


echo "Запуск тестов с разными приоритетами..."
pids=()
for i in "${!EXECUTABLES[@]}"; do
    exe="${EXECUTABLES[$i]}"
    prio="${DIFFERENT_PRIORITIES[$i]}"
    echo "Запуск $exe с приоритетом $prio"
    sudo taskset -c 0 "$exe" "$prio" &
    pids+=($!)
done
wait_for_processes "${pids[@]}"


