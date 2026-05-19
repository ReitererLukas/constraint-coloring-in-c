CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion \
           -Woverloaded-virtual -Wnon-virtual-dtor -Wdelete-non-virtual-dtor \
           -Wformat=2 -Winit-self -Wunused-variable -Wunused-function \
           -std=c++17 -O2
SRCDIR = src
OBJDIR = build
BINDIR = .
TESTDIR = tests
TESTBIN = $(BINDIR)/testrunner

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/constraints

# Test sources (exclude main.cpp from linking)
TEST_SOURCES = $(filter-out $(SRCDIR)/main.cpp, $(SOURCES))
TEST_OBJECTS = $(TEST_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TEST_OBJECTS += $(OBJDIR)/test_runner_main.o

.PHONY: all clean run test

%:
	@:

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) $(filter-out $@,$(MAKECMDGOALS))

test: $(TESTBIN)
	./$(TESTBIN)

$(TESTBIN): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(TESTBIN)

$(OBJDIR)/test_runner_main.o: $(TESTDIR)/test_runner.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET) $(TESTBIN)

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all   - Build the program (default)"
	@echo "  run   - Build and run the program"
	@echo "  test  - Build and run tests"
	@echo "  clean - Remove built files"
	@echo "  help  - Show this help message"
