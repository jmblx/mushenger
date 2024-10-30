#!/bin/bash

# Проверяем наличие файла .env
if [ -f .env ]; then
    # Экспортируем переменные из .env в окружение
    export $(grep -v '^#' .env | xargs)
    echo "Variables loaded from .env"
else
    echo ".env file not found!"
fi

