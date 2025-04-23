#!/bin/bash

echo "$(pwd)"
# Компиляция
gcc src/2/2.2/2.2-father.c -o father
gcc src/2/2.2/2.2-son.c -o son

# Проверка компиляции
if [[ ! -f father || ! -f son ]]; then
    echo "Ошибка компиляции!"
    exit 1
fi

# Запуск в обычном режиме
echo "=== ЗАПУСК В ОБЫЧНОМ РЕЖИМЕ ==="
./father


# Запуск в фоновом режиме
echo -e "\n=== ЗАПУСК В ФОНОВОМ РЕЖИМЕ ==="
# ./father 
setsid ./father </dev/null &>/dev/null &
father_pid=$!
echo "Родительский процесс запущен в фоне (PID: $father_pid)"
set -x
ps -el | grep father
{
set +x;
} 2>/dev/null


# Ожидание завершения 
sleep 5
if ps -p $father_pid >/dev/null; then
    echo "Фоновый процесс все еще работает (PID: $father_pid)"
else
    echo "Процесс завершился"
fi

# Удаление бинарных файлов
rm -f father son