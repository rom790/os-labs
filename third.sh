#!/bin/sh

# Проверка наличия аргумента
if [ $# -lt 2 ]; then
    echo "Ошибка"
    exit 1
fi

# Файл для сохранения результатов
output_file=$2
> "$output_file"

# Получение абсолютного пути к целевому файлу
target_file=$(realpath "$1")

# Поиск символьных ссылок, указывающих на целевой файл
find /home -type l 2>/dev/null | while read -r symlink; do
    if [ "$(realpath  "$symlink" 2>/dev/null)" = "$target_file" ]; then
        echo "$symlink"
    fi
done > "$output_file"

# Подсчет количества найденных ссылок
count=$(wc -l < "$output_file")

# Добавление количества ссылок в файл
echo "Всего: $count" >> "$output_file"

# Вывод результата
echo "Результаты сохранены в $output_file"
echo "Найдено символьных ссылок: $count"