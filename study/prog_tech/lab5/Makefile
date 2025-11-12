
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/bookstore
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/Base.cpp $(SRCDIR)/Book.cpp $(SRCDIR)/Textbook.cpp $(SRCDIR)/Stationery.cpp $(SRCDIR)/Keeper.cpp
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
HEADERS = $(SRCDIR)/include/Base.h $(SRCDIR)/include/Book.h $(SRCDIR)/include/Textbook.h $(SRCDIR)/include/Stationery.h $(SRCDIR)/include/Keeper.h $(SRCDIR)/include/my_vector.h $(SRCDIR)/include/json.hpp


all: $(BUILDDIR) $(TARGET)


$(BUILDDIR):
	mkdir -p $(BUILDDIR)


$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)


$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -rf $(BUILDDIR) *.txt


run: $(TARGET)
	$(TARGET)

