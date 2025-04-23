#! /bin/sh

if [ $# -lt 2 ]; then
  echo "$0: Ошибка"
  exit 1
fi

output_file=$2
> "$output_file"

filename=$1
inode=`ls -i $filename | awk '{print $1}' | tr -d " "`
    #awk '{print $1}'
    #Извлекает первый столбец из вывода ls -i (inode).
    #-d ' ' указывает, что разделитель — пробел.
    #tr -d " ":
    #Удаляет все пробелы из строки (на случай, если inode содержит пробелы).

tmp=$(ls -lRi ~ | grep $inode)
# ls -li получает все файлы
# grep $inode выбирает из всех файлов только те, которые содераж нужный inode



if [ -n "$tmp" ]; then
  echo "$tmp" >> "$output_file"
  echo "Файл найден. Результаты сохранены в $output_file."
fi