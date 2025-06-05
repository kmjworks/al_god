# Valgrind, Makefile, and Logging Cheat Sheet - C Programming

## Valgrind - Memory Debugging Tool

### What is Valgrind?
Valgrind is a programming tool for memory debugging, memory leak detection, and profiling. The most commonly used tool is Memcheck.

### Basic Usage
```bash
# Compile with debugging symbols
gcc -g -o program program.c

# Run with valgrind
valgrind ./program

# Run with detailed leak checking
valgrind --leak-check=full ./program

# Run with track origins for uninitialized values
valgrind --leak-check=full --track-origins=yes ./program
```

### Common Valgrind Options
```bash
# Full command with common options
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./program args
```

### Valgrind Output Interpretation

#### Memory Leak Types
1. **Definitely lost**: Memory that was allocated but not freed and is no longer reachable
2. **Indirectly lost**: Memory that was only reachable through definitely lost memory
3. **Possibly lost**: Memory that might be lost (pointers to middle of blocks)
4. **Still reachable**: Memory that was not freed but is still reachable at exit

#### Common Errors
```c
// Invalid read/write
int *arr = malloc(5 * sizeof(int));
arr[5] = 10;  // Invalid write of size 4

// Use of uninitialized value
int x;
if (x > 0) { }  // Conditional jump depends on uninitialised value

// Invalid free
free(arr);
free(arr);  // Invalid free() / delete / delete[] / realloc()

// Memory leak
int *ptr = malloc(sizeof(int));
// Missing free(ptr)
```

### Valgrind Suppressions
Create a suppression file for known issues:
```
{
   <insert_a_suppression_name_here>
   Memcheck:Leak
   match-leak-kinds: reachable
   fun:malloc
   obj:/usr/lib/x86_64-linux-gnu/libstdc++.so.6
}
```

Use: `valgrind --suppressions=suppressions.txt ./program`

## Makefile - Build Automation

### Basic Makefile Structure
```makefile
# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = program
OBJECTS = main.o utils.o math.o

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files
main.o: main.c utils.h math.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

math.o: math.c math.h
	$(CC) $(CFLAGS) -c math.c

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
```

### Advanced Makefile Features

#### Automatic Variables
```makefile
# $@ - target name
# $< - first prerequisite
# $^ - all prerequisites
# $* - stem of pattern rule

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
```

#### Pattern Rules
```makefile
# Generic rule for all .c to .o
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Generic rule for all object files
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@
```

#### Conditional Compilation
```makefile
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS += -g -DDEBUG
else
    CFLAGS += -O2
endif
```

#### Complete Example Makefile
```makefile
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm  # Math library

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Files
TARGET = $(BIN_DIR)/myapp
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)

# Debug/Release modes
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS += -g -DDEBUG
    TARGET := $(TARGET)_debug
else
    CFLAGS += -O2 -DNDEBUG
endif

# Default target
all: directories $(TARGET)

# Create directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile with dependency generation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependencies
-include $(DEPS)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Install
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Run tests
test: $(TARGET)
	./run_tests.sh

# Valgrind check
valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes $(TARGET)

.PHONY: all clean install test valgrind directories
```

### Makefile Best Practices
1. Use variables for repeated values
2. Include dependency tracking with `-MMD -MP`
3. Create separate directories for objects and binaries
4. Use pattern rules to avoid repetition
5. Always include a clean target
6. Mark non-file targets as .PHONY

## Logging in C

### Basic Logging Implementation

#### Simple Logging Macros
```c
#include <stdio.h>
#include <time.h>
#include <string.h>

// Log levels
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
} LogLevel;

// Global log level
static LogLevel current_log_level = LOG_INFO;

// Log level names
static const char* log_level_names[] = {
    "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

// Basic logging macro
#define LOG(level, fmt, ...) do { \
    if (level >= current_log_level) { \
        time_t now = time(NULL); \
        char timestr[20]; \
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", localtime(&now)); \
        fprintf(stderr, "[%s] [%s] %s:%d: " fmt "\n", \
                timestr, log_level_names[level], __FILE__, __LINE__, ##__VA_ARGS__); \
    } \
} while(0)

// Convenience macros
#define LOG_DEBUG(fmt, ...) LOG(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG(LOG_INFO, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG(LOG_WARNING, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(LOG_ERROR, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG(LOG_FATAL, fmt, ##__VA_ARGS__)
```

#### Advanced Logging System
```c
// logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    LOG_TRACE = 0,
    LOG_DEBUG = 1,
    LOG_INFO = 2,
    LOG_WARN = 3,
    LOG_ERROR = 4,
    LOG_FATAL = 5
} LogLevel;

typedef struct {
    LogLevel level;
    FILE *file;
    bool use_color;
    bool include_timestamp;
    bool include_location;
    void (*custom_handler)(LogLevel level, const char *msg);
} Logger;

// Initialize logger
void logger_init(Logger *logger, LogLevel level, const char *filename);
void logger_close(Logger *logger);

// Logging functions
void logger_log(Logger *logger, LogLevel level, const char *file, 
                int line, const char *fmt, ...);

// Macros for easy use
#define LOG_TRACE(logger, fmt, ...) \
    logger_log(logger, LOG_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(logger, fmt, ...) \
    logger_log(logger, LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(logger, fmt, ...) \
    logger_log(logger, LOG_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(logger, fmt, ...) \
    logger_log(logger, LOG_WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(logger, fmt, ...) \
    logger_log(logger, LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(logger, fmt, ...) \
    logger_log(logger, LOG_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif // LOGGER_H
```

#### Rotating Log Files
```c
#include <sys/stat.h>

typedef struct {
    char base_filename[256];
    size_t max_size;
    int max_files;
    FILE *current_file;
    size_t current_size;
} RotatingLogger;

void rotate_log_file(RotatingLogger *logger) {
    if (logger->current_file) {
        fclose(logger->current_file);
    }
    
    // Rename existing files
    for (int i = logger->max_files - 1; i > 0; i--) {
        char old_name[300], new_name[300];
        sprintf(old_name, "%s.%d", logger->base_filename, i - 1);
        sprintf(new_name, "%s.%d", logger->base_filename, i);
        rename(old_name, new_name);
    }
    
    // Rename current file
    char backup_name[300];
    sprintf(backup_name, "%s.0", logger->base_filename);
    rename(logger->base_filename, backup_name);
    
    // Open new file
    logger->current_file = fopen(logger->base_filename, "w");
    logger->current_size = 0;
}

void write_rotating_log(RotatingLogger *logger, const char *message) {
    size_t msg_len = strlen(message);
    
    if (logger->current_size + msg_len > logger->max_size) {
        rotate_log_file(logger);
    }
    
    fprintf(logger->current_file, "%s", message);
    fflush(logger->current_file);
    logger->current_size += msg_len;
}
```

### Creating a Static Library

#### Library Creation Steps
```bash
# 1. Create object files
gcc -c -Wall -Werror -fpic logger.c
gcc -c -Wall -Werror -fpic utils.c

# 2. Create static library
ar rcs libmylib.a logger.o utils.o

# 3. Use the library
gcc main.c -L. -lmylib -o program

# 4. Create shared library (alternative)
gcc -shared -o libmylib.so logger.o utils.o

# 5. Use shared library
gcc main.c -L. -lmylib -o program
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./program
```

#### Library Makefile Example
```makefile
# Library Makefile
LIB_NAME = mylib
VERSION = 1.0.0

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -fPIC
AR = ar
ARFLAGS = rcs

# Files
SOURCES = logger.c utils.c config.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = logger.h utils.h config.h

# Targets
STATIC_LIB = lib$(LIB_NAME).a
SHARED_LIB = lib$(LIB_NAME).so.$(VERSION)
SHARED_LINK = lib$(LIB_NAME).so

# Build both libraries
all: $(STATIC_LIB) $(SHARED_LIB)

# Static library
$(STATIC_LIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

# Shared library
$(SHARED_LIB): $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(SHARED_LINK) -o $@ $^
	ln -sf $(SHARED_LIB) $(SHARED_LINK)

# Object files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Install
install: all
	mkdir -p /usr/local/lib /usr/local/include
	cp $(STATIC_LIB) $(SHARED_LIB) /usr/local/lib/
	cp $(HEADERS) /usr/local/include/
	ldconfig

# Clean
clean:
	rm -f $(OBJECTS) $(STATIC_LIB) $(SHARED_LIB) $(SHARED_LINK)

.PHONY: all clean install
```

## Portable Logging Example

```c
// portable_logger.h
#ifndef PORTABLE_LOGGER_H
#define PORTABLE_LOGGER_H

#include <stdio.h>
#include <stdarg.h>

// Platform detection
#ifdef _WIN32
    #define PLATFORM_WINDOWS
    #include <windows.h>
#else
    #define PLATFORM_UNIX
    #include <unistd.h>
    #include <sys/time.h>
#endif

// ANSI color codes (Unix only)
#ifdef PLATFORM_UNIX
    #define COLOR_RED     "\x1b[31m"
    #define COLOR_GREEN   "\x1b[32m"
    #define COLOR_YELLOW  "\x1b[33m"
    #define COLOR_BLUE    "\x1b[34m"
    #define COLOR_MAGENTA "\x1b[35m"
    #define COLOR_CYAN    "\x1b[36m"
    #define COLOR_RESET   "\x1b[0m"
#else
    #define COLOR_RED     ""
    #define COLOR_GREEN   ""
    #define COLOR_YELLOW  ""
    #define COLOR_BLUE    ""
    #define COLOR_MAGENTA ""
    #define COLOR_CYAN    ""
    #define COLOR_RESET   ""
#endif

// Thread-safe logging
#ifdef PLATFORM_WINDOWS
    #define MUTEX_TYPE CRITICAL_SECTION
    #define MUTEX_INIT(m) InitializeCriticalSection(&(m))
    #define MUTEX_LOCK(m) EnterCriticalSection(&(m))
    #define MUTEX_UNLOCK(m) LeaveCriticalSection(&(m))
    #define MUTEX_DESTROY(m) DeleteCriticalSection(&(m))
#else
    #include <pthread.h>
    #define MUTEX_TYPE pthread_mutex_t
    #define MUTEX_INIT(m) pthread_mutex_init(&(m), NULL)
    #define MUTEX_LOCK(m) pthread_mutex_lock(&(m))
    #define MUTEX_UNLOCK(m) pthread_mutex_unlock(&(m))
    #define MUTEX_DESTROY(m) pthread_mutex_destroy(&(m))
#endif

typedef struct {
    FILE *file;
    int level;
    int use_colors;
    MUTEX_TYPE mutex;
} PortableLogger;

void portable_logger_init(PortableLogger *logger, const char *filename);
void portable_logger_log(PortableLogger *logger, int level, 
                        const char *fmt, ...);
void portable_logger_close(PortableLogger *logger);

#endif // PORTABLE_LOGGER_H
```

## Quick Reference

### Valgrind Commands
| Command | Purpose |
|---------|---------|
| `valgrind ./program` | Basic memory check |
| `--leak-check=full` | Detailed leak information |
| `--track-origins=yes` | Track uninitialized values |
| `--show-leak-kinds=all` | Show all leak types |
| `--gen-suppressions=all` | Generate suppressions |

### Makefile Variables
| Variable | Meaning |
|----------|---------|
| `$@` | Target name |
| `$<` | First prerequisite |
| `$^` | All prerequisites |
| `$*` | Stem of pattern rule |
| `$(wildcard *.c)` | All .c files |
| `$(patsubst %.c,%.o,$(SOURCES))` | Pattern substitution |

### Logging Best Practices
1. Use different log levels appropriately
2. Include timestamps and source location
3. Make logging thread-safe if needed
4. Implement log rotation for long-running programs
5. Allow runtime configuration of log levels
6. Use conditional compilation to remove debug logs in release

## Exam Tips
1. Valgrind requires `-g` flag during compilation for meaningful output
2. Memory leaks show as "definitely lost" in valgrind
3. Makefile targets should be marked .PHONY if they're not files
4. Use automatic variables in Makefile to avoid repetition
5. Logging should be designed to have minimal performance impact
6. Static libraries (.a) are linked at compile time
7. Shared libraries (.so) are linked at runtime