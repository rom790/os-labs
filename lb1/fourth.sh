#!/bin/bash

# Проверяем, был ли передан параметр (путь к файлу)
if [ -z "$1" ]; then
  echo "Укажите путь к файлу."
  exit 1
fi

target_file="$1"

# Способ 1: Использование find и stat
echo "Поиск с использованием find и stat:"
find ~ -type l -exec stat -c "%N" {} \; 2>/dev/null | grep "$target_file"
echo -e "\n"

# Способ 2: Использование find и -lname
echo "Поиск с использованием find и -lname:"
find ~ -type l -lname "*$target_file*" 2>/dev/null
echo -e "\n"

# Способ 3: Использование readlink в цикле
echo "Поиск с использованием readlink в цикле:"
for link in $(find ~ -type l 2>/dev/null); do
  if readlink "$link" | grep -q "$target_file"; then
    echo "$link -> $(readlink "$link")"
  fi
done
echo -e "\n"

# Способ 4: Использование ls с фильтрацией через grep
echo "Поиск с использованием ls и grep:"
ls -lRa ~ | grep " -> " | grep "$target_file"
echo -e "\n"

# Способ 5: Использование find и realpath
echo "Поиск с использованием find и realpath:"
find ~ -type l 2>/dev/null | while read -r symlink; do
  if [ "$(realpath 2>/dev/null "$symlink")" = "$(realpath 2>/dev/null "$target_file")" ]; then
    echo "$symlink"
  fi
done
echo -e "\n"

# Способ 6: Использование find и readlink -f
echo "Поиск с использованием find и readlink -f:"
find ~ -type l 2>/dev/null | while read -r symlink; do
  if [ "$(readlink -f "$symlink")" = "$(readlink -f "$target_file")" ]; then
    echo "$symlink"
  fi
done
echo -e "\n"
