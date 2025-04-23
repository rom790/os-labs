#!/bin/bash


direct=$1

# Функция для вывода размера каталога
get_dir_size() {
    echo -e "Размер каталога: $(du -sh $direct | cut -f1) байт\n"
}

# Начальный размер каталога
echo "Начальный размер каталога:"
get_dir_size

# Добавляем 10 файлов по 1 МБ
echo "Добавляем 10 файлов по 1 МБ..."
for i in {1..10}; do
    dd if=/dev/zero of=$direct/file_$i bs=1b count=1 &>/dev/null
done
get_dir_size

# Добавляем 10 подкаталогов
echo "Добавляем 10 подкаталогов..."
for i in {1..10}; do
    mkdir -p $direct/subdir_$i
done
get_dir_size

# Удаляем 5 файлов
echo "Удаляем 5 файлов..."
for i in {1..5}; do
    rm $direct/file_$i
done
get_dir_size

# Удаляем 5 подкаталогов
echo "Удаляем 5 подкаталогов..."
for i in {1..5}; do
    rmdir $direct/subdir_$i
done
get_dir_size
