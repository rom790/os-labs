#!/bin/bash

if [ $# -lt 1 ]; then
  echo "$0: Ошибка"
  exit 1
fi

output_file="$1"
> "$output_file"

echo "Поиск..."

sudo ls -lR / 2>/dev/null | awk '
/^\/.*:$/ { current_dir = substr($0, 1, length($0)-1); next }  # Запоминаем текущую директорию
NF >= 9 {
    type = substr($1, 1, 1);  # Извлекаем первый символ — тип файла
    if (!(type in seen)) {     # Если этот тип ещё не встречался
        file = (current_dir == "/" ? "" : current_dir) "/" $NF;    # Формируем путь к файлу
        print $1, file;
        seen[type] = 1;        # Запоминаем, что этот тип уже найден
    }
}' >> "$output_file"

echo "Записано в $output_file"