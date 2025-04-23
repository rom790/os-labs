# #!/bin/bash

# # Функция для проверки, является ли процесс системным
# is_system_process() {
#     local flags=$1

#     # Если флаг нечетный, процесс системный
#     if (( (flags & 1) == 1 )) 2>/dev/null; then
#         return 0
#     fi

#     return 1
# }

# # Получаем список всех процессов
# echo "Анализ процессов..."
# echo ""

# # Выводим заголовки с выравниванием
# printf "%-10s %-15s %-30s %-1s\n" "PID" "Пользователь" "Команда" "Тип"

# # Используем команду ps для получения информации о процессах, включая флаги
# ps -eo pid,user,cmd,flags | while read -r pid user cmd flags; do
#     # awk '{print $10}'
#     # Пропускаем заголовок
#     if [[ "$pid" == "PID" ]]; then
#         continue
#     fi
#     # Проверяем, является ли процесс системным
#     if is_system_process "$flags"; then
#         printf "%-10s %-12s %-50s %-10s\n" "$pid" "$user" "$cmd" "Системный"
#     else
#         printf "%-10s %-12s %-50s %-10s\n" "$pid" "$user" "$cmd" "Пользовательский"
#     fi
# done


#!/bin/bash

# Функция для проверки, является ли процесс системным
is_system_process() {
    local flags=$1

    # Если флаг нечетный, процесс системный
    if (( (flags & 1) == 1 )) 2>/dev/null; then
        return 0
    fi

    return 1
}

# Временный файл для хранения результатов
output_file="process_report_$(date +%Y%m%d_%H%M%S).txt"

echo "Анализ процессов..."
echo ""

# Заголовки для вывода
header="%-10s %-15s %-30s %-1s\n"
divider="=============================================================="

# Записываем результаты в файл
{
    printf "$header" "PID" "Пользователь" "Команда" "Тип"
    echo "$divider"

    ps -eo pid,user,cmd,flags | while read -r pid user cmd flags; do
        # Пропускаем заголовок
        if [[ "$pid" == "PID" ]]; then
            continue
        fi
        
        # Определяем тип процесса
        if is_system_process "$flags"; then
            printf "$header" "$pid" "$user" "$cmd" "Системный"
        else
            printf "$header" "$pid" "$user" "$cmd" "Пользовательский"
        fi
    done
} > "$output_file"

# echo "Результаты сохранены в файл: $output_file"
# echo ""

# Выводим примеры процессов
echo "Пример 10 системных процессов:"
echo "$divider"
grep "Системный" "$output_file" | head -n 10
echo ""

echo "Пример 10 пользовательских процессов:"
echo "$divider"
grep "Пользовательский" "$output_file" | head -n 10
echo ""

rm $output_file

