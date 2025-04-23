
EXECUTABLES=("src/5/5.1/5.1-1" "src/5/5.1/5.1-2" "src/5/5.1/5.1-3" "src/5/5.1/5.1-4")
DIFFERENT_NICE=(-20 -10 0 10)
SAME_NICE=(0 0 0 0)

run_test() {
    local -n NICE_ARRAY=$1
    local test_label=$2

    echo "Запуск теста: $test_label"

    # Очистка файлов с выводом
    for executable in "${EXECUTABLES[@]}"; do
        local outfile="output_$(basename "$executable")_${test_label}.log"
        > "$outfile"
    done

    # Запуск процессов
    declare -a pids=()
    for i in "${!EXECUTABLES[@]}"; do
        local exe="${EXECUTABLES[$i]}"
        local outfile="output_$(basename "$exe")_${test_label}.log"
        taskset -c 0 nice -n "${NICE_ARRAY[$i]}" ./"$exe" >> "$outfile" 2>&1 &
        pids+=($!)
    done

    sleep 5

    for pid in "${pids[@]}"; do
        kill "$pid" 2>/dev/null
    done

    declare -A counts
    for executable in "${EXECUTABLES[@]}"; do
        local outfile="output_$(basename "$executable")_${test_label}.log"
        counts["$executable"]=$(wc -l < "$outfile")
    done

    local total_count=0
    for count in "${counts[@]}"; do
        ((total_count += count))
    done

    echo "Результаты теста: $test_label"
    [ "$total_count" -eq 0 ] && total_count=1

    for i in "${!EXECUTABLES[@]}"; do
        local exe="${EXECUTABLES[$i]}"
        local count="${counts[$exe]}"
        local percent=$(awk -v c="$count" -v t="$total_count" 'BEGIN {printf "%.2f", c/t*100}')
        echo "Процесс $exe выполнился $count раз (nice=${NICE_ARRAY[$i]}) - $percent%"
    done
    
}

# Тесты
run_test SAME_NICE "Одинаковые значения nice"
run_test DIFFERENT_NICE "Разные значения nice"

# Очистка
for executable in "${EXECUTABLES[@]}"; do
    rm -f "output_$(basename "$executable")_Одинаковые значения nice.log"
    rm -f "output_$(basename "$executable")_Разные значения nice.log"
done