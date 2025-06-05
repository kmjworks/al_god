# Qsort and Libraries Cheat Sheet - C Programming

## qsort() Function Overview
`qsort()` is a standard library function that implements the quicksort algorithm to sort arrays of any type.

### Function Signature
```c
#include <stdlib.h>

void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));
```

### Parameters
- `base`: Pointer to the first element of the array
- `nmemb`: Number of elements in the array
- `size`: Size of each element in bytes
- `compar`: Pointer to comparison function

## Comparison Function

### Basic Structure
```c
int compare(const void *a, const void *b) {
    // Cast pointers to appropriate type
    // Return:
    //   < 0 if a should come before b
    //   = 0 if a and b are equal
    //   > 0 if a should come after b
}
```

## Common qsort() Examples

### 1. Sorting Integers
```c
// Ascending order
int compare_int_asc(const void *a, const void *b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return int_a - int_b;
}

// Descending order
int compare_int_desc(const void *a, const void *b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return int_b - int_a;
}

// Usage
int numbers[] = {5, 2, 8, 1, 9};
qsort(numbers, 5, sizeof(int), compare_int_asc);
```

### 2. Sorting Floating Point Numbers
```c
int compare_double(const void *a, const void *b) {
    double diff = *(const double*)a - *(const double*)b;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

// Alternative safe comparison
int compare_double_safe(const void *a, const void *b) {
    const double *da = (const double*)a;
    const double *db = (const double*)b;
    return (*da > *db) - (*da < *db);
}
```

### 3. Sorting Strings
```c
// Sort array of strings
int compare_strings(const void *a, const void *b) {
    const char **str_a = (const char**)a;
    const char **str_b = (const char**)b;
    return strcmp(*str_a, *str_b);
}

// Sort array of char arrays
int compare_char_arrays(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

// Usage
char *words[] = {"zebra", "apple", "mango", "banana"};
qsort(words, 4, sizeof(char*), compare_strings);

char names[][20] = {"John", "Alice", "Bob", "Charlie"};
qsort(names, 4, sizeof(names[0]), compare_char_arrays);
```

### 4. Sorting Structures
```c
typedef struct {
    char name[50];
    int age;
    float salary;
} Employee;

// Sort by age
int compare_by_age(const void *a, const void *b) {
    const Employee *emp_a = (const Employee*)a;
    const Employee *emp_b = (const Employee*)b;
    return emp_a->age - emp_b->age;
}

// Sort by name
int compare_by_name(const void *a, const void *b) {
    const Employee *emp_a = (const Employee*)a;
    const Employee *emp_b = (const Employee*)b;
    return strcmp(emp_a->name, emp_b->name);
}

// Sort by salary (descending)
int compare_by_salary_desc(const void *a, const void *b) {
    const Employee *emp_a = (const Employee*)a;
    const Employee *emp_b = (const Employee*)b;
    return (emp_b->salary > emp_a->salary) - (emp_b->salary < emp_a->salary);
}

// Multi-level sort (primary: age, secondary: name)
int compare_multi_level(const void *a, const void *b) {
    const Employee *emp_a = (const Employee*)a;
    const Employee *emp_b = (const Employee*)b;
    
    int age_diff = emp_a->age - emp_b->age;
    if (age_diff != 0) return age_diff;
    
    return strcmp(emp_a->name, emp_b->name);
}
```

### 5. Sorting Pointers
```c
// Sort array of integer pointers by pointed values
int compare_int_ptrs(const void *a, const void *b) {
    int *ptr_a = *(int**)a;
    int *ptr_b = *(int**)b;
    return *ptr_a - *ptr_b;
}

// Sort with NULL handling
int compare_with_null(const void *a, const void *b) {
    int *ptr_a = *(int**)a;
    int *ptr_b = *(int**)b;
    
    if (ptr_a == NULL && ptr_b == NULL) return 0;
    if (ptr_a == NULL) return 1;  // NULLs at end
    if (ptr_b == NULL) return -1;
    
    return *ptr_a - *ptr_b;
}
```

## Standard Libraries

### 1. stdlib.h - General Utilities
```c
#include <stdlib.h>

// Memory allocation
void* malloc(size_t size);
void* calloc(size_t nmemb, size_t size);
void* realloc(void *ptr, size_t size);
void free(void *ptr);

// Random numbers
int rand(void);
void srand(unsigned int seed);

// String conversion
int atoi(const char *str);
long atol(const char *str);
double atof(const char *str);
long strtol(const char *str, char **endptr, int base);

// Program control
void exit(int status);
void abort(void);

// Environment
char* getenv(const char *name);
int system(const char *command);

// Searching and sorting
void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));
void* bsearch(const void *key, const void *base,
              size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));
```

### 2. stdio.h - Input/Output
```c
#include <stdio.h>

// File operations
FILE* fopen(const char *filename, const char *mode);
int fclose(FILE *stream);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

// Formatted I/O
int printf(const char *format, ...);
int scanf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int sscanf(const char *str, const char *format, ...);

// Character I/O
int getchar(void);
int putchar(int c);
char* fgets(char *s, int size, FILE *stream);
int fputs(const char *s, FILE *stream);
```

### 3. string.h - String Operations
```c
#include <string.h>

// String manipulation
char* strcpy(char *dest, const char *src);
char* strncpy(char *dest, const char *src, size_t n);
char* strcat(char *dest, const char *src);
char* strncat(char *dest, const char *src, size_t n);

// String examination
size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char* strchr(const char *s, int c);
char* strrchr(const char *s, int c);
char* strstr(const char *haystack, const char *needle);

// Memory operations
void* memcpy(void *dest, const void *src, size_t n);
void* memmove(void *dest, const void *src, size_t n);
void* memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
```

### 4. math.h - Mathematical Functions
```c
#include <math.h>

// Trigonometric
double sin(double x);
double cos(double x);
double tan(double x);
double asin(double x);
double acos(double x);
double atan(double x);
double atan2(double y, double x);

// Exponential and logarithmic
double exp(double x);
double log(double x);
double log10(double x);
double pow(double x, double y);
double sqrt(double x);

// Rounding
double ceil(double x);
double floor(double x);
double round(double x);
double trunc(double x);

// Other
double fabs(double x);
double fmod(double x, double y);
```

### 5. time.h - Date and Time
```c
#include <time.h>

// Time functions
time_t time(time_t *tloc);
double difftime(time_t time1, time_t time0);
struct tm* localtime(const time_t *timep);
struct tm* gmtime(const time_t *timep);
char* asctime(const struct tm *tm);
char* ctime(const time_t *timep);
size_t strftime(char *s, size_t max, const char *format,
                const struct tm *tm);

// Clock
clock_t clock(void);
```

## Creating Your Own Library

### 1. Header File (mylib.h)
```c
#ifndef MYLIB_H
#define MYLIB_H

// Function declarations
int my_max(int a, int b);
double calculate_average(const int *arr, int size);
void print_array(const int *arr, int size);

// Type definitions
typedef struct {
    int x;
    int y;
} Point;

double distance(Point p1, Point p2);

#endif // MYLIB_H
```

### 2. Implementation File (mylib.c)
```c
#include "mylib.h"
#include <stdio.h>
#include <math.h>

int my_max(int a, int b) {
    return (a > b) ? a : b;
}

double calculate_average(const int *arr, int size) {
    if (size <= 0) return 0.0;
    
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return (double)sum / size;
}

void print_array(const int *arr, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

double distance(Point p1, Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}
```

### 3. Creating a Static Library
```bash
# Compile source file to object file
gcc -c mylib.c -o mylib.o

# Create static library
ar rcs libmylib.a mylib.o

# Use the library
gcc main.c -L. -lmylib -o program
```

### 4. Creating a Shared Library
```bash
# Compile with position-independent code
gcc -c -fPIC mylib.c -o mylib.o

# Create shared library
gcc -shared -o libmylib.so mylib.o

# Use the library
gcc main.c -L. -lmylib -o program

# Run (may need to set LD_LIBRARY_PATH)
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./program
```

## Advanced qsort() Techniques

### 1. Stable Sort Implementation
```c
typedef struct {
    void *data;
    size_t original_index;
} StableElement;

int stable_compare(const void *a, const void *b, 
                  int (*original_compare)(const void *, const void *)) {
    const StableElement *elem_a = (const StableElement*)a;
    const StableElement *elem_b = (const StableElement*)b;
    
    int result = original_compare(elem_a->data, elem_b->data);
    if (result == 0) {
        // Maintain original order for equal elements
        return elem_a->original_index - elem_b->original_index;
    }
    return result;
}
```

### 2. Context-Aware Sorting (using qsort_r)
```c
// GNU extension (not standard C)
typedef struct {
    int sort_column;
    bool ascending;
} SortContext;

int compare_with_context(const void *a, const void *b, void *context) {
    SortContext *ctx = (SortContext*)context;
    // Use context for sorting decisions
}
```

### 3. Partial Sorting
```c
// Sort only first k elements
void partial_qsort(void *base, size_t nmemb, size_t size, size_t k,
                   int (*compar)(const void *, const void *)) {
    if (k >= nmemb) {
        qsort(base, nmemb, size, compar);
        return;
    }
    
    // Custom implementation for partial sort
    // (Full implementation would be more complex)
}
```

## Quick Reference

### qsort() Template
```c
// 1. Define comparison function
int compare_type(const void *a, const void *b) {
    const Type *ta = (const Type*)a;
    const Type *tb = (const Type*)b;
    // Return comparison result
}

// 2. Call qsort
qsort(array, num_elements, sizeof(Type), compare_type);
```

### Common Patterns
| Data Type | Comparison | Return Statement |
|-----------|------------|------------------|
| int | Ascending | `return *(int*)a - *(int*)b;` |
| double | Safe | `return (*a > *b) - (*a < *b);` |
| string | Alphabetical | `return strcmp(*(char**)a, *(char**)b);` |
| struct | By field | `return ((Type*)a)->field - ((Type*)b)->field;` |

## Exam Tips
1. Remember the void* parameters in comparison functions
2. Cast pointers to correct types before dereferencing
3. Return values: negative (a<b), zero (a==b), positive (a>b)
4. For floating-point, avoid simple subtraction
5. Include necessary headers: `<stdlib.h>` for qsort
6. Size parameter is `sizeof(element)`, not total array size
7. String arrays need different handling than char* arrays