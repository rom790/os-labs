#!/bin/bash

# Получаем PID текущей оболочки
PID=$$

# Проверяем, существует ли процесс с данным PID
if ! [ -d /proc/$PID ]; then
    echo "Ошибка: процесс с PID $PID не найден."
    exit 1
fi

# Функция для вывода информации с разделением на блоки
print_section() {
    local title=$1
    local file_path=$2
    local transform_cmd=$3

    echo -e "\n====== $title ======"
    if [ -f "$file_path" ]; then

        cat "$file_path" | $transform_cmd
    else
        echo "Файл не найден: $file_path"
    fi
}

# Выводим основную информацию о процессе
print_section "Информация о процессе c PID $PID" "/proc/$PID/status" "cat"

# Выводим командную строку процесса
print_section "Командная строка процесса" "/proc/$PID/cmdline" "tr '\0' '\t'"

# Выводим переменные окружения
print_section "Переменные окружения" "/proc/$PID/environ" "tr '\0' '\n'"

# Выводим открытые файлы
# print_section "Открытые файлы" "/proc/$PID/fd" "ls -l"
echo ""
echo "====== Открытые файлы (10 верхних) ======"
echo "/proc/$PID/fd: $(sudo ls -l /proc/1/fd | head -n 10)"
