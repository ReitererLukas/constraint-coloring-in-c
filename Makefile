CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion \
           -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wdelete-non-virtual-dtor \
           -Wformat=2 -Winit-self -Wunused-variable -Wunused-function \
           -std=c++17 -O2
SRCDIR = src
OBJDIR = build
BINDIR = .

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/constraints

.PHONY: all clean run

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all   - Build the program (default)"
	@echo "  run   - Build and run the program"
	@echo "  clean - Remove built files"
	@echo "  help  - Show this help message"
