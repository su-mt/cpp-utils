
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/bookstore
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/Base.cpp $(SRCDIR)/Book.cpp $(SRCDIR)/Textbook.cpp $(SRCDIR)/Stationery.cpp $(SRCDIR)/Keeper.cpp
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
HEADERS = $(SRCDIR)/Base.h $(SRCDIR)/Book.h $(SRCDIR)/Textbook.h $(SRCDIR)/Stationery.h $(SRCDIR)/Keeper.h


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


.PHONY: all clean run debug deps