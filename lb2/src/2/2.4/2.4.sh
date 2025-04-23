#!/bin/bash

files=("src/2/2.4/2.4-a" "src/2/2.4/2.4-b" "src/2/2.4/2.4-c")
for file in "${files[@]}"
do
    echo "------------------------" >> out.txt
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Запуск программы: $file" >> out.txt

    # Проверяем, существует ли файл
    if [[ -f "./$file" ]]; then
        ./$file >> out.txt  # Запускаем программу и записываем вывод в out.txt
    else
        echo "Файл $file не найден." >> out.txt
        continue
    fi

    # Вывод информации о процессах
    echo "Состояние процессов после запуска $file:" >> out.txt
    ps -l>> out.txt

    # Если это 2.4-b, выполняем дополнительные действия
    if [[ "$file" == "lb2/2/2.4/2.4-b" ]]; then
        echo "" >> out.txt

        # Ищем процесс 2.4-b в выводе ps и извлекаем его PPID
        ppid=$(ps -o pid,ppid,comm f | grep "2.4-b" | awk '{print $2}')
        if [[ -n "$ppid" ]]; then
            echo "Родительский PID для 2.4-b: $ppid" >> out.txt
            echo "Процесс с PID $ppid:" >> out.txt
            ps aux | grep "$ppid" | grep -v grep >> out.txt
        else
            echo "Процесс 2.4-b не найден." >> out.txt
        fi
    fi

    echo "------------------------" >> out.txt
done