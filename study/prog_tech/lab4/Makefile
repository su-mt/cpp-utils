
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g


SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
BUILD_DIR = build


SOURCES = $(SRC_DIR)/main.cpp
TARGET = $(BUILD_DIR)/lab4


all: $(TARGET)


$(TARGET): $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $(SOURCES) -o $(TARGET)


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)


.PHONY: all run clean help
