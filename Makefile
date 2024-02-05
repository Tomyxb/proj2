# Compiler and linker
CC := g++
# The directory of header files
INCLUDES := -I include
# Compiler flags
CFLAGS := -Wall $(INCLUDES)
# Source directory for main application
SRCDIR := src
# Source directory for tests
TESTSRCDIR := testsrc
# Object files directory
OBJDIR := obj
# Binary files directory
BINDIR := bin

# Find all the CPP files in the source directory
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
# Define object files for all sources
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
# Find all the CPP files in the test source directory
TESTSOURCES := $(wildcard $(TESTSRCDIR)/*.cpp)
# Define object files for all test sources
TESTOBJECTS := $(patsubst $(TESTSRCDIR)/%.cpp, $(OBJDIR)/%.o, $(TESTSOURCES))

# Define the main target
MAIN := $(BINDIR)/main
# Define the test target
TEST := $(BINDIR)/test

# Default target
all: directories $(MAIN)

# Create the object and binary directories
directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

# Link the main application
$(MAIN): $(OBJECTS)
	$(CC) $^ -o $@

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | directories
	$(CC) $(CFLAGS) -c $< -o $@

# Target for building tests
test: directories $(TEST)

# Link the test cases
$(TEST): $(TESTOBJECTS)
	$(CC) $^ -o $@

# Compile test source files into object files
$(OBJDIR)/%.o: $(TESTSRCDIR)/%.cpp | directories
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*

.PHONY: all test clean directories
