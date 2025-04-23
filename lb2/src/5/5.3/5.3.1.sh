#!/bin/bash

# Пути к исполняемым файлам
EXECUTABLES=("src/5/5.3/5.3-1" "src/5/5.3/5.3-2" "src/5/5.3/5.3-3")

# Приоритет для запуска с одинаковыми приоритетами
SAME_PRIORITY=10


# Функция для ожидания завершения группы процессов
wait_for_processes() {
    for pid in "$@"; do
        wait "$pid"
    done
}

echo "Запуск процессов с одинаковыми приоритетами ($SAME_PRIORITY)..."
pids=()
for exe in "${EXECUTABLES[@]}"; do
    echo "Запуск $exe с приоритетом $SAME_PRIORITY"
    sudo taskset -c 0 "$exe" "$SAME_PRIORITY" &
    pids+=($!)
done

wait_for_processes "${pids[@]}"




