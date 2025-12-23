# Makefile for running unit tests

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
INCLUDES = -Isrc
LIBS = -lm

# Directories
SRCDIR = src
TESTDIR = tests
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
TESTOBJDIR = $(OBJDIR)/test

# Source files for the main application
SOURCES = $(wildcard $(SRCDIR)/*.c)
# Exclude main.c since we have our own test main
SOURCES := $(filter-out $(SRCDIR)/main.c, $(SOURCES))
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Test files
TEST_SOURCES = $(wildcard $(TESTDIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.c=$(TESTOBJDIR)/%.o)

# Test executable
TEST_TARGET = $(BUILDDIR)/test_runner

# Create necessary directories
$(OBJDIR) $(TESTOBJDIR) $(BUILDDIR):
	mkdir -p $@

# Build test executable
$(TEST_TARGET): $(OBJECTS) $(TEST_OBJECTS) | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) $(TEST_OBJECTS) -o $@ $(LIBS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile test files to object files
$(TESTOBJDIR)/%.o: $(TESTDIR)/%.c | $(TESTOBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run tests
.PHONY: test
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean test build artifacts
.PHONY: test-clean
test-clean:
	rm -f $(TEST_TARGET)

# Phony targets
.PHONY: test test-clean