#!/bin/bash
# Пути к исполняемым файлам
echo -1 | sudo tee /proc/sys/kernel/sched_rt_runtime_us
echo ""
echo "5.6.1 Запуск потоков с одинаковыми приоритетами, но разными политиками..."
echo ""
# pids=()
# for exe in "${EXECUTABLES[@]}"; do
sudo taskset -c 0 "src/5/5.6/5.6.1" &
    # pids+=($!)
# done

sleep 15

echo ""
echo "5.6.2 Запуск потоков с одинаковыми политиками, но разными приоритетами..."
echo ""
sudo taskset -c 0 "src/5/5.6/5.6.2" &

sleep 15

echo 950000 | sudo tee /proc/sys/kernel/sched_rt_runtime_us





