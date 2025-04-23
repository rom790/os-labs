#!/bin/bash

# Пути к исполняемым файлам
EXECUTABLES=("src/5/5.5/5.5-other" "src/5/5.5/5.5-fifo" "src/5/5.5/5.5-rr")

# Приоритет для запуска с одинаковыми приоритетами
SAME_PRIORITY=10


# Функция для ожидания завершения группы процессов
wait_for_processes() {
    for pid in "$@"; do
        wait "$pid"
    done
}

echo "Запуск тестов с разными политиками и одинаковыми приоритетами ($SAME_PRIORITY)..."
pids=()
for exe in "${EXECUTABLES[@]}"; do
    echo "Запуск $exe с приоритетом $SAME_PRIORITY"
    sudo taskset -c 0 "$exe" "$SAME_PRIORITY" &
    pids+=($!)
done

sleep 20

for pid in "${pids[@]}"; do
    kill "$pid" 2>/dev/null
done





