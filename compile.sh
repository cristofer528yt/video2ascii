#!/bin/bash

# Script для компиляции video2ascii

echo "Компиляция video2ascii..."

# Проверяем наличие OpenCV
if ! pkg-config --exists opencv4; then
    echo "OpenCV не найден. Устанавливаем зависимости..."
    
    # Проверяем систему и устанавливаем OpenCV
    if command -v pacman &> /dev/null; then
        # Arch Linux
        echo "Обнаружен Arch Linux"
        if ! pacman -Qi opencv > /dev/null 2>&1; then
            echo "Установка OpenCV..."
            sudo pacman -S --noconfirm opencv
        fi
    elif command -v apt &> /dev/null; then
        # Debian/Ubuntu
        echo "Обнаружен Debian/Ubuntu"
        if ! dpkg -l | grep -q libopencv-dev; then
            echo "Установка OpenCV..."
            sudo apt update && sudo apt install -y libopencv-dev
        fi
    else
        echo "Не удалось определить пакетный менеджер. Установите OpenCV вручную."
        echo "Для Arch: sudo pacman -S opencv"
        echo "Для Debian/Ubuntu: sudo apt install libopencv-dev"
        exit 1
    fi
fi

# Компилируем программу с минимальными зависимостями
echo "Компиляция программы..."
g++ -std=c++11 video2ascii.cpp -o video2ascii \
    $(pkg-config --cflags --libs opencv4) \
    -O2 -DNDEBUG

# Альтернативная команда компиляции если предыдущая не работает
if [ $? -ne 0 ]; then
    echo "Попытка компиляции с альтернативными флагами..."
    g++ -std=c++11 video2ascii.cpp -o video2ascii \
        -I/usr/include/opencv4 \
        -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_highgui \
        -O2 -DNDEBUG
fi

# Проверяем успешность компиляции
if [ $? -eq 0 ]; then
    echo "Компиляция успешна!"
    echo "Бинарный файл: $(pwd)/video2ascii"
    echo ""
    echo "Использование: ./video2ascii --video video_file.mp4"
else
    echo "Ошибка компиляции!"
    echo "Попробуйте установить opencv вручную:"
    echo "sudo pacman -S opencv"
    exit 1
fi
