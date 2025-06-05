# Complete Makefile Example
# This demonstrates various Makefile features

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests
LIB_DIR = lib

# Target executable
TARGET = $(BIN_DIR)/myapp

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)

# Test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/test_%.o,$(TEST_SOURCES))
TEST_TARGET = $(BIN_DIR)/test_runner

# Library
LIB_NAME = mylib
STATIC_LIB = $(LIB_DIR)/lib$(LIB_NAME).a

# Debug/Release configuration
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS += -g -O0 -DDEBUG
    TARGET := $(TARGET)_debug
else
    CFLAGS += -O2 -DNDEBUG
endif

# Verbose output
VERBOSE ?= 0
ifeq ($(VERBOSE), 0)
    Q = @
    ECHO = @echo
else
    Q =
    ECHO = @true
endif

# Colors for output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

# Default target
all: directories $(TARGET)
	$(ECHO) "$(GREEN)Build complete!$(NC)"

# Create necessary directories
directories:
	$(Q)mkdir -p $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

# Main target
$(TARGET): $(OBJECTS)
	$(ECHO) "Linking $@..."
	$(Q)$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files with automatic dependency generation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(ECHO) "Compiling $<..."
	$(Q)$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Static library
lib: directories $(STATIC_LIB)

$(STATIC_LIB): $(OBJECTS)
	$(ECHO) "Creating static library $@..."
	$(Q)ar rcs $@ $^

# Test compilation
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c
	$(ECHO) "Compiling test $<..."
	$(Q)$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# Test target
test: directories $(TEST_TARGET)
	$(ECHO) "Running tests..."
	$(Q)$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(OBJ_DIR)/main.o,$(OBJECTS))
	$(ECHO) "Linking tests..."
	$(Q)$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Run the program
run: $(TARGET)
	$(ECHO) "$(YELLOW)Running $(TARGET)...$(NC)"
	$(Q)$(TARGET)

# Valgrind memory check
valgrind: $(TARGET)
	$(ECHO) "$(YELLOW)Running Valgrind memory check...$(NC)"
	$(Q)valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --verbose $(TARGET)

# Generate documentation
docs:
	$(ECHO) "Generating documentation..."
	$(Q)doxygen Doxyfile

# Static analysis
analyze:
	$(ECHO) "Running static analysis..."
	$(Q)cppcheck --enable=all --suppress=missingIncludeSystem \
		$(SRC_DIR) $(TEST_DIR)

# Format code
format:
	$(ECHO) "Formatting code..."
	$(Q)find $(SRC_DIR) $(TEST_DIR) -name "*.c" -o -name "*.h" | \
		xargs clang-format -i

# Clean build artifacts
clean:
	$(ECHO) "$(RED)Cleaning build artifacts...$(NC)"
	$(Q)rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

# Clean everything including generated files
distclean: clean
	$(ECHO) "$(RED)Cleaning all generated files...$(NC)"
	$(Q)rm -rf docs/ tags cscope.out

# Install the program
install: $(TARGET)
	$(ECHO) "Installing $(TARGET)..."
	$(Q)install -d $(DESTDIR)/usr/local/bin
	$(Q)install -m 755 $(TARGET) $(DESTDIR)/usr/local/bin

# Uninstall
uninstall:
	$(ECHO) "Uninstalling..."
	$(Q)rm -f $(DESTDIR)/usr/local/bin/$(notdir $(TARGET))

# Generate tags for code navigation
tags:
	$(ECHO) "Generating tags..."
	$(Q)ctags -R $(SRC_DIR) $(TEST_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the main program (default)"
	@echo "  lib       - Build static library"
	@echo "  test      - Build and run tests"
	@echo "  run       - Run the program"
	@echo "  valgrind  - Run with Valgrind memory checker"
	@echo "  clean     - Remove build artifacts"
	@echo "  distclean - Remove all generated files"
	@echo "  install   - Install the program"
	@echo "  docs      - Generate documentation"
	@echo "  analyze   - Run static analysis"
	@echo "  format    - Format source code"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Options:"
	@echo "  DEBUG=1   - Build with debug symbols"
	@echo "  VERBOSE=1 - Show detailed build commands"

# Include dependency files
-include $(DEPS)

# Phony targets
.PHONY: all directories clean distclean install uninstall run test \
        valgrind docs analyze format tags help lib

# Secondary expansion for pattern rules
.SECONDEXPANSION:

# Keep intermediate files
.PRECIOUS: $(OBJ_DIR)/%.o

# Default goal
.DEFAULT_GOAL := all