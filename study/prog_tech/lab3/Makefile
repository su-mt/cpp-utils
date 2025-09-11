# Makefile для проекта с геометрическими фигурами


CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2


SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = $(SRC_DIR)/include


SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/circle.cpp \
          $(SRC_DIR)/rectangle.cpp \
          $(SRC_DIR)/trapezoid.cpp \
          $(SRC_DIR)/triangle.cpp \
          $(SRC_DIR)/menu.cpp


OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)


TARGET = $(BUILD_DIR)/figures


all: $(TARGET)


$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


run: $(TARGET)
	./$(TARGET)


clean:
	rm -rf $(BUILD_DIR)


rebuild: clean all


debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)



.PHONY: all run clean rebuild debug info
