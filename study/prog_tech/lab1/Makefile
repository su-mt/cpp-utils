# Makefile для проекта очередей

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude
TARGET = lab1
OBJECTS = main.o queue.o queue1_public.o queue1_protected.o queue1_private.o

# Основная цель
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Компиляция основных файлов
main.o: main.cpp include/queue.h include/queue1_public.h include/queue1_protected.h include/queue1_private.h
	$(CXX) $(CXXFLAGS) -c main.cpp

queue.o: queue.cpp include/queue.h
	$(CXX) $(CXXFLAGS) -c queue.cpp

queue1_public.o: queue1_public.cpp include/queue1_public.h include/queue.h
	$(CXX) $(CXXFLAGS) -c queue1_public.cpp

queue1_protected.o: queue1_protected.cpp include/queue1_protected.h include/queue.h
	$(CXX) $(CXXFLAGS) -c queue1_protected.cpp

queue1_private.o: queue1_private.cpp include/queue1_private.h include/queue.h
	$(CXX) $(CXXFLAGS) -c queue1_private.cpp

# Очистка
clean:
	rm -f $(OBJECTS) $(TARGET)

# Перекомпиляция
rebuild: clean $(TARGET)

run:
	./$(TARGET)

.PHONY: clean rebuild run
