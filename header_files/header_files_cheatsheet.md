# Header Files Cheat Sheet - C Programming

## What are Header Files?
Header files (.h) contain declarations and macro definitions to be shared between source files. They allow modular programming by separating interface (declarations) from implementation.

## Basic Structure

### Simple Header File
```c
// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Function declarations
int add(int a, int b);
double calculate_area(double radius);

// Macro definitions
#define PI 3.14159265359
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif // MATH_UTILS_H
```

### Corresponding Source File
```c
// math_utils.c
#include "math_utils.h"

int add(int a, int b) {
    return a + b;
}

double calculate_area(double radius) {
    return PI * radius * radius;
}
```

## Include Guards

### Traditional Include Guards
```c
#ifndef MYHEADER_H
#define MYHEADER_H

// Header content here

#endif // MYHEADER_H
```

### Pragma Once (Non-standard but widely supported)
```c
#pragma once

// Header content here
```

## Types of Includes

### System Headers
```c
#include <stdio.h>      // Standard I/O
#include <stdlib.h>     // Standard library
#include <string.h>     // String operations
#include <math.h>       // Math functions
```

### User Headers
```c
#include "myheader.h"   // Current directory first
#include "../lib/util.h" // Relative path
#include "subdir/module.h" // Subdirectory
```

## What to Put in Headers

### DO Include:
```c
// 1. Function declarations (prototypes)
int process_data(const char *input, int size);

// 2. Type definitions
typedef struct {
    int x;
    int y;
} Point;

// 3. Structure declarations
struct Node {
    int data;
    struct Node *next;
};

// 4. Macro definitions
#define BUFFER_SIZE 1024
#define VERSION "1.0.0"

// 5. Enum definitions
typedef enum {
    STATUS_OK,
    STATUS_ERROR,
    STATUS_PENDING
} Status;

// 6. External variable declarations
extern int global_counter;

// 7. Inline functions (C99)
static inline int square(int x) {
    return x * x;
}
```

### DON'T Include:
```c
// 1. Function definitions (except inline)
int add(int a, int b) {  // NO! Put in .c file
    return a + b;
}

// 2. Variable definitions
int counter = 0;  // NO! This causes multiple definition errors

// 3. Large static data
static int huge_array[10000] = {0};  // NO! Duplicated in each file
```

## Complete Example

### Project Structure
```
project/
├── main.c
├── utils/
│   ├── string_utils.h
│   ├── string_utils.c
│   ├── math_utils.h
│   └── math_utils.c
└── data/
    ├── list.h
    └── list.c
```

### string_utils.h
```c
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>  // for size_t

// Function declarations
char* str_trim(char *str);
int str_starts_with(const char *str, const char *prefix);
char* str_duplicate(const char *str);
void str_reverse(char *str);

// Macro for string length with null check
#define SAFE_STRLEN(s) ((s) ? strlen(s) : 0)

#endif // STRING_UTILS_H
```

### math_utils.h
```c
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Constants
#define PI 3.14159265359
#define E 2.71828182846

// Macros
#define SQUARE(x) ((x) * (x))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

// Type definitions
typedef struct {
    double x;
    double y;
} Vector2D;

typedef struct {
    double real;
    double imag;
} Complex;

// Function declarations
double distance(Vector2D a, Vector2D b);
Vector2D vector_add(Vector2D a, Vector2D b);
Complex complex_multiply(Complex a, Complex b);

// Inline functions
static inline double deg_to_rad(double degrees) {
    return degrees * PI / 180.0;
}

static inline double rad_to_deg(double radians) {
    return radians * 180.0 / PI;
}

#endif // MATH_UTILS_H
```

### list.h
```c
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

// Forward declaration
typedef struct Node Node;
typedef struct List List;

// Opaque pointer (implementation hidden)
struct List;

// Constructor/Destructor
List* list_create(void);
void list_destroy(List *list);

// Operations
bool list_append(List *list, int value);
bool list_prepend(List *list, int value);
bool list_insert(List *list, int index, int value);
bool list_remove(List *list, int index);

// Access
int list_get(const List *list, int index);
int list_size(const List *list);
bool list_is_empty(const List *list);

// Iteration
typedef void (*ListCallback)(int value, void *context);
void list_foreach(const List *list, ListCallback callback, void *context);

#endif // LIST_H
```

## Advanced Header Techniques

### 1. Conditional Compilation
```c
#ifndef CONFIG_H
#define CONFIG_H

// Platform detection
#ifdef _WIN32
    #define PLATFORM "Windows"
    #include <windows.h>
#elif __linux__
    #define PLATFORM "Linux"
    #include <unistd.h>
#elif __APPLE__
    #define PLATFORM "macOS"
    #include <unistd.h>
#endif

// Debug mode
#ifdef DEBUG
    #define LOG(msg) printf("[DEBUG] %s\n", msg)
    #define ASSERT(cond) if(!(cond)) { fprintf(stderr, "Assertion failed: %s\n", #cond); abort(); }
#else
    #define LOG(msg)
    #define ASSERT(cond)
#endif

#endif // CONFIG_H
```

### 2. Function-like Macros with Type Safety
```c
#ifndef GENERIC_UTILS_H
#define GENERIC_UTILS_H

// Type-generic max
#define GENERIC_MAX(type) \
    static inline type max_##type(type a, type b) { \
        return (a > b) ? a : b; \
    }

// Generate functions
GENERIC_MAX(int)
GENERIC_MAX(float)
GENERIC_MAX(double)

// Safe array size
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// Stringify macro
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#endif // GENERIC_UTILS_H
```

### 3. API Versioning
```c
#ifndef MY_API_H
#define MY_API_H

#define API_VERSION_MAJOR 2
#define API_VERSION_MINOR 1
#define API_VERSION_PATCH 0

#define API_VERSION_STRING TOSTRING(API_VERSION_MAJOR) "." \
                          TOSTRING(API_VERSION_MINOR) "." \
                          TOSTRING(API_VERSION_PATCH)

// Deprecation warnings
#ifdef __GNUC__
    #define DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
    #define DEPRECATED(msg) __declspec(deprecated(msg))
#else
    #define DEPRECATED(msg)
#endif

// Old API (deprecated)
DEPRECATED("Use new_function instead")
void old_function(void);

// New API
void new_function(void);

#endif // MY_API_H
```

### 4. Header-only Libraries
```c
// single_file_lib.h
#ifndef SINGLE_FILE_LIB_H
#define SINGLE_FILE_LIB_H

// API declarations
void lib_function(void);

// Implementation
#ifdef SINGLE_FILE_LIB_IMPLEMENTATION

void lib_function(void) {
    printf("Library function called\n");
}

#endif // SINGLE_FILE_LIB_IMPLEMENTATION
#endif // SINGLE_FILE_LIB_H

// Usage in ONE .c file:
// #define SINGLE_FILE_LIB_IMPLEMENTATION
// #include "single_file_lib.h"
```

## Common Patterns

### 1. Callback Functions
```c
#ifndef CALLBACKS_H
#define CALLBACKS_H

// Callback type definitions
typedef void (*EventHandler)(int event_id, void *data);
typedef int (*Comparator)(const void *a, const void *b);
typedef void* (*Allocator)(size_t size);
typedef void (*Deallocator)(void *ptr);

// Register callbacks
void register_event_handler(EventHandler handler);
void set_memory_functions(Allocator alloc, Deallocator dealloc);

#endif // CALLBACKS_H
```

### 2. Error Handling
```c
#ifndef ERROR_H
#define ERROR_H

// Error codes
typedef enum {
    ERR_SUCCESS = 0,
    ERR_INVALID_PARAM = -1,
    ERR_OUT_OF_MEMORY = -2,
    ERR_FILE_NOT_FOUND = -3,
    ERR_PERMISSION_DENIED = -4
} ErrorCode;

// Error handling
extern ErrorCode last_error;
const char* error_to_string(ErrorCode err);
void set_error(ErrorCode err);

// Macro for error checking
#define CHECK_ERROR(expr) do { \
    ErrorCode err = (expr); \
    if (err != ERR_SUCCESS) { \
        fprintf(stderr, "Error at %s:%d: %s\n", \
                __FILE__, __LINE__, error_to_string(err)); \
        return err; \
    } \
} while(0)

#endif // ERROR_H
```

### 3. Module Interface
```c
#ifndef MODULE_H
#define MODULE_H

// Module initialization/cleanup
int module_init(void);
void module_cleanup(void);

// Module configuration
typedef struct {
    int option1;
    char *option2;
    bool option3;
} ModuleConfig;

int module_configure(const ModuleConfig *config);

// Module operations
int module_process(const void *input, void *output);
int module_get_status(void);

#endif // MODULE_H
```

## Best Practices

1. **Always use include guards**
2. **Keep headers minimal** - only declarations, not definitions
3. **Use forward declarations** when possible to reduce dependencies
4. **Document public APIs** in headers
5. **Group related functionality** in the same header
6. **Avoid circular dependencies**
7. **Use `const` for read-only parameters**
8. **Provide clear naming** with prefixes for public functions

## Common Mistakes

### 1. Multiple Definitions
```c
// BAD: Definition in header
int counter = 0;  // Error: multiple definitions

// GOOD: Declaration in header, definition in .c
extern int counter;  // In .h
int counter = 0;     // In .c
```

### 2. Missing Include Guards
```c
// BAD: No guards
void function(void);

// GOOD: With guards
#ifndef HEADER_H
#define HEADER_H
void function(void);
#endif
```

### 3. Circular Dependencies
```c
// a.h includes b.h
// b.h includes a.h  // Circular!

// Solution: Use forward declarations
// In a.h:
struct B;  // Forward declaration
struct A {
    struct B *b_ptr;
};
```

## Quick Reference
| Element | Header File | Source File |
|---------|------------|-------------|
| Function declaration | ✓ | ✗ |
| Function definition | ✗ (except inline) | ✓ |
| Macro definition | ✓ | ✗ |
| Type definition | ✓ | ✗ |
| Structure declaration | ✓ | ✗ |
| External variable | ✓ (extern) | ✓ (definition) |
| Static variable | ✗ | ✓ |
| Include guards | ✓ | ✗ |

## Exam Tips
1. Include guards prevent multiple inclusion
2. Use angle brackets < > for system headers, quotes " " for user headers
3. Headers contain declarations, source files contain definitions
4. `extern` declares without defining
5. Forward declarations can break circular dependencies
6. Static inline functions can be defined in headers
7. Macros don't respect scope - use carefully