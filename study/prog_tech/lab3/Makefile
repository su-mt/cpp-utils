# Makefile для проекта с геометрическими фигурами

# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2

# Директории
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = $(SRC_DIR)/include

# Исходные файлы
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/circle.cpp \
          $(SRC_DIR)/rectangle.cpp \
          $(SRC_DIR)/trapezoid.cpp \
          $(SRC_DIR)/triangle.cpp \
          $(SRC_DIR)/menu.cpp

# Объектные файлы
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Название исполняемого файла
TARGET = $(BUILD_DIR)/figures

# Основная цель
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@

# Компиляция объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

# Создание директории build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Очистка
clean:
	rm -rf $(BUILD_DIR)

# Перестройка проекта
rebuild: clean all

# Отладочная сборка
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Информация о проекте
info:
	@echo "Проект: Геометрические фигуры"
	@echo "Компилятор: $(CXX)"
	@echo "Флаги: $(CXXFLAGS)"
	@echo "Исходные файлы: $(SOURCES)"
	@echo "Цель: $(TARGET)"

.PHONY: all run clean rebuild debug info
