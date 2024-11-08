# Этап 1: Сборка приложения
FROM ubuntu:latest as builder

# Устанавливаем зависимости для сборки
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    wget \
    libgl1-mesa-dev \
    libxkbcommon-x11-0 \
    qtbase5-dev

# Создаем рабочую директорию
WORKDIR /app

# Копируем локальные файлы проекта из контекста сборки в контейнер
COPY . .

# Переходим в папку сборки и компилируем проект
RUN mkdir /app/build && cd /app/build && qmake /app/Server.pro && make

# Этап 2: Финальный образ с зависимостями
FROM ubuntu:latest

# Устанавливаем библиотеки Qt, необходимые для запуска
RUN apt-get update && \
    apt-get install -y \
    libgl1-mesa-dev \
    libxkbcommon-x11-0 \
    qtbase5-dev && \
    rm -rf /var/lib/apt/lists/*  # Удаляем кеш APT для уменьшения размера образа

# Копируем только собранные артефакты из предыдущего этапа
COPY --from=builder /app/build /release-server

# Устанавливаем рабочую директорию
WORKDIR /release-server
