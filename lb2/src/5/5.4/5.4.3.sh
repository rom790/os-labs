#!/bin/bash

# Пути к исполняемым файлам
EXECUTABLES=("src/5/5.4/5.4-1" "src/5/5.4/5.4-2" "src/5/5.4/5.4-3" "src/5/5.4/5.4-4")



# Приоритеты для запуска с разными приоритетами (для каждого из 3-х процессов)
DIFFERENT_PRIORITIES=(95 95 10 40)

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

sleep 15

for pid in "${pids[@]}"; do
    kill "$pid" 2>/dev/null
done




