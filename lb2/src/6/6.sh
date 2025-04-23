#!/bin/bash


EXECUTABLES=("src/6/6.1" "src/6/6.2" "src/6/6.3")


echo "Запуск тестов 6 задания..."
pids=()
for exe in "${EXECUTABLES[@]}"; do
    sudo taskset -c 0 "$exe" 30 30 &
    pids+=($!)
    sleep 2
done
echo ""
echo "6.4 Наследование через clone"
echo ""
echo "clone() позволяет точно контролировать, какие ресурсы разделяются между процессами, а какие копируются, через флаги"
echo "CLONE_FILES Общие файловые дескрипторы
CLONE_FS Общая файловая система (корень, CWD, umask)
CLONE_VM Общее адресное пространство (память)
CLONE_SIGHAND Общие обработчики сигналов"
echo ""
sleep 5
echo "==============================================================="
echo ""
echo "----------Содержимое файла fork.txt----------"
cat fork.txt
echo ""
echo "----------Содержимое файла exec.txt----------"
cat exec.txt
rm -f fork.txt
