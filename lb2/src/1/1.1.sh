#!/bin/bash
set -euo pipefail

# Функция для очистки фоновых процессов при завершении скрипта
cleanup() {
  for pid in "${job_pids[@]:-}"; do
    if kill -0 "$pid" 2>/dev/null; then
      echo "Останавливаем процесс с PID $pid"
      kill -9 "$pid" 2>/dev/null || true # Жесткий сигнал для завершения
    fi
  done
}

# Перехватывает сигнал EXIT для вызова очистки
trap cleanup EXIT

echo "Запуск фоновых процессов..."

# Массив для хранения PID запущенных процессов
job_pids=()

# Задание 1: Поиск файлов в директории (find)
echo "Задание 1: длительный sleep (20)"
sleep 20 &
job1_pid=$!
job_pids+=("$job1_pid")
echo "Задание 1 (sleep 20) запущено с PID $job1_pid"


# Задание 2: Подсчет строк в файле (wc)
echo "Задание 2: короткий sleep (5)"
sleep 5 &
job2_pid=$!
job_pids+=("$job2_pid")
echo "Задание 2 (sleep 5) запущено с PID $job2_pid"


# Задание 3: Копирование файла (cp)
echo "Задание 3: средний sleep (15)"
sleep 15 &
job3_pid=$!
job_pids+=("$job3_pid")
echo "Задание 3 (sleep 15) запущено с PID $job3_pid"


echo -e "\nАнализ списка заданий:"
set -x
date '+%Y-%m-%d %H:%M:%S'
jobs -l
{
set +x;
} 2>/dev/null

# Прерывание задания 2 (wc), если оно еще выполняется
echo -e "\nПрерывание задания 2 (sleep 5)..."
if kill -0 "$job2_pid" 2>/dev/null; then
  kill -9 "$job2_pid" 2>/dev/null && echo "Задание 2 было прервано." || echo "Не удалось прервать задание 2."
else
  echo "Задание 2 уже завершилось или его нельзя прервать."
fi
sleep 1 # Даем время для обновления статуса

echo -e "\nАнализ списка заданий после прерывания задания 2 (sleep 5):"
set -x
date '+%Y-%m-%d %H:%M:%S'
jobs -l
{
set +x;
} 2>/dev/null

# Ожидание завершения задания 1 
echo -e "\nОжидание завершения задания 1 (sleep 20)..."
if wait "$job1_pid"; then
  echo "Задание 1 успешно завершено."
else
  echo "Задание 1 завершилось с ошибкой."
fi

echo -e "\nИтоговый анализ списка заданий:"
set -x
date '+%Y-%m-%d %H:%M:%S'
jobs -l
{
set +x;
} 2>/dev/null
echo -e "\nСкрипт завершен."
