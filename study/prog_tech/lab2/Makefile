TARGET := build/lab2
SRC := src/main.cpp src/queue.cpp src/long_number.cpp
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
INCLUDES := -Isrc

$(TARGET): $(SRC)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

.PHONY: all clean run
all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build

