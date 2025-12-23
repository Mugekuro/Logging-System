# Makefile for Payment & Data Management System

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -O2

# Debug flags (uncomment for debugging)
# CFLAGS += -g -DDEBUG

# Directories
SRCDIR = src
OBJDIR = build/obj
BINDIR = build/bin
DATADIR = data

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/payment_system

# Default target
all: directories $(TARGET)

# Create directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR) $(DATADIR)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Linking complete: $@"

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(SRCDIR)
	@echo "Compiled: $< -> $@"

# Clean build artifacts
clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(BINDIR)/*
	@echo "Build artifacts cleaned"

# Clean all including data
clean-all: clean
	rm -rf $(DATADIR)/*
	@echo "All artifacts cleaned"

# Rebuild
rebuild: clean all

# Run the program
run: all
	./$(TARGET)

# Install (copy executable to system path - Linux/Mac only)
install: all
	@echo "Installing to system..."
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete"

# Uninstall (remove executable from system path - Linux/Mac only)
uninstall:
	@echo "Uninstalling from system..."
	@sudo rm -f /usr/local/bin/payment_system
	@echo "Uninstallation complete"

# Print build information
info:
	@echo "Build Information:"
	@echo "  Compiler: $(CC)"
	@echo "  Flags: $(CFLAGS)"
	@echo "  Sources: $(SOURCES)"
	@echo "  Objects: $(OBJECTS)"
	@echo "  Target: $(TARGET)"

# Backup data
backup:
	@echo "Creating backup of data directory..."
	@mkdir -p backup/$(shell date +%Y%m%d_%H%M%S)
	@cp -r $(DATADIR)/* backup/$(shell date +%Y%m%d_%H%M%S)/ 2>/dev/null || echo "No data to backup"
	@echo "Backup complete"

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove build artifacts"
	@echo "  clean-all  - Remove build artifacts and data"
	@echo "  rebuild    - Clean and rebuild the project"
	@echo "  run        - Build and run the program"
	@echo "  install    - Install the program to system (Linux/Mac)"
	@echo "  uninstall  - Remove the program from system (Linux/Mac)"
	@echo "  info       - Print build information"
	@echo "  backup     - Backup data directory"
	@echo "  help       - Show this help message"

# Phony targets
.PHONY: all clean clean-all rebuild run install uninstall info backup help