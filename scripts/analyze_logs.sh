#!/bin/bash

# Инструмент анализа логов Text Analyzer

LOG_DIR="/var/log/text_analyzer"

# Проверяем, есть ли логи
if [ ! -d "$LOG_DIR" ]; then
    echo "Директория логов не найдена: $LOG_DIR"
    exit 1
fi

echo "=== АНАЛИЗ ЛОГОВ Text Analyzer ==="
echo

# 1. Общее количество запусков
echo "1. Общее количество запусков:"
awk 'END {print "   Всего запусков:", NR}' $LOG_DIR/*.log
echo

# 2. Запуски по дням
echo "2. Распределение по дням:"
awk -F'[_ ]' '{print $1}' $LOG_DIR/*.log | sort | uniq -c
echo

# 3. Запуски по часам
echo "3. Распределение по часам:"
awk -F'[ :]' '{print $3}' $LOG_DIR/*.log | sort | uniq -c
echo

# 4. Количество обработанных файлов (для утилиты)
echo "4. Обработанные файлы:"
grep -h "Stats:" $LOG_DIR/*.log | wc -l
echo

# 5. Список ошибок
echo "5. Список ошибок:"
grep "ERROR" $LOG_DIR/*.log || echo "   Ошибок не найдено"
echo

echo "Анализ завершен."