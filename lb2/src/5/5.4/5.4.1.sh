#!/bin/bash

# Пути к исполняемым файлам
EXECUTABLES=("src/5/5.4/5.4-1" "src/5/5.4/5.4-2" "src/5/5.4/5.4-3" "src/5/5.4/5.4-4")

# Приоритет для запуска с одинаковыми приоритетами
SAME_PRIORITY=10



echo "Запуск тестов с одинаковыми приоритетами ($SAME_PRIORITY)..."
pids=()
for exe in "${EXECUTABLES[@]}"; do
    echo "Запуск $exe с приоритетом $SAME_PRIORITY"
    sudo taskset -c 0 "$exe" "$SAME_PRIORITY" &
    pids+=($!)
done

sleep 15

for pid in "${pids[@]}"; do
    kill "$pid" 2>/dev/null
done


