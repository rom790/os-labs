#!/bin/bash

# Проверка на наличие аргумента с путём
if [ $# -eq 0 ]; then
  echo "Usage: $0 <directory>"
  exit 1
fi

# Директория для поиска
directory="$1"

# Найдем самый длинный путь
longest_path=$(find "$directory" -type f | awk '{print length, $0}' | sort -n | tail -n 1 | cut -d' ' -f2-)

# Выведем результат
echo "Самый длинный путь: $longest_path"
echo "Длина пути: $(echo "$longest_path" | wc -c)"