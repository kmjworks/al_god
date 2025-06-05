# Dynamic Memory Allocation Cheat Sheet - C Programming

## Overview
Dynamic memory allocation allows programs to request memory at runtime from the heap. Unlike stack memory, dynamically allocated memory persists until explicitly freed.

## Memory Layout
```
High Address
┌─────────────────┐
│   Command Line  │
│   Arguments     │
├─────────────────┤
│   Stack         │ ← Local variables, function parameters
│   ↓             │   (grows downward)
├─────────────────┤
│                 │
│   Free Space    │
│                 │
├─────────────────┤
│   ↑             │   (grows upward)
│   Heap          │ ← Dynamic memory allocation
├─────────────────┤
│   BSS Segment   │ ← Uninitialized global/static variables
├─────────────────┤
│   Data Segment  │ ← Initialized global/static variables
├─────────────────┤
│   Text Segment  │ ← Program code
└─────────────────┘
Low Address
```

## Memory Allocation Functions

### malloc() - Memory Allocation
```c
void* malloc(size_t size);
```
- Allocates `size` bytes of uninitialized memory
- Returns pointer to allocated memory or NULL if allocation fails
- Memory contents are undefined (garbage values)

### calloc() - Contiguous Allocation
```c
void* calloc(size_t num, size_t size);
```
- Allocates memory for array of `num` elements of `size` bytes each
- Initializes all bytes to zero
- Returns pointer to allocated memory or NULL if allocation fails

### free() - Deallocate Memory
```c
void free(void* ptr);
```
- Releases memory pointed to by `ptr`
- `ptr` must have been returned by malloc/calloc/realloc
- After free, `ptr` becomes invalid (dangling pointer)
- Freeing NULL pointer is safe (no operation)

## Basic Usage Examples

### 1. Simple Allocation
```c
// Allocate single integer
int *num = (int*)malloc(sizeof(int));
if (num == NULL) {
    printf("Memory allocation failed\n");
    return -1;
}
*num = 42;
printf("Value: %d\n", *num);
free(num);
num = NULL;  // Good practice
```

### 2. Array Allocation
```c
// Method 1: Using malloc
int n = 10;
int *arr1 = (int*)malloc(n * sizeof(int));

// Method 2: Using calloc (initialized to 0)
int *arr2 = (int*)calloc(n, sizeof(int));

// Check allocation
if (arr1 == NULL || arr2 == NULL) {
    free(arr1);  // Safe even if NULL
    free(arr2);
    return -1;
}

// Use arrays
for (int i = 0; i < n; i++) {
    arr1[i] = i * 2;      // Uninitialized, must set
    printf("%d ", arr2[i]); // Will print 0s
}

free(arr1);
free(arr2);
```

### 3. String Allocation
```c
// Allocate string
char *str = (char*)malloc(100 * sizeof(char));
if (str == NULL) {
    return -1;
}

// Safe string operations
strncpy(str, "Hello, Dynamic Memory!", 99);
str[99] = '\0';  // Ensure null termination

printf("%s\n", str);
free(str);

// Alternative: Duplicate string
char *original = "Hello World";
char *copy = (char*)malloc(strlen(original) + 1);
if (copy) {
    strcpy(copy, original);
    printf("%s\n", copy);
    free(copy);
}
```

### 4. Structure Allocation
```c
typedef struct {
    char name[50];
    int age;
    float salary;
} Person;

// Single structure
Person *p1 = (Person*)malloc(sizeof(Person));
if (p1 == NULL) return -1;

strcpy(p1->name, "John Doe");
p1->age = 30;
p1->salary = 50000.0;

// Array of structures
int count = 5;
Person *team = (Person*)calloc(count, sizeof(Person));
if (team == NULL) {
    free(p1);
    return -1;
}

// Initialize array
for (int i = 0; i < count; i++) {
    sprintf(team[i].name, "Employee %d", i + 1);
    team[i].age = 25 + i;
    team[i].salary = 40000 + i * 5000;
}

free(p1);
free(team);
```

## Advanced Allocation Patterns

### 1. 2D Array - Contiguous Memory
```c
// Allocate as single block
int rows = 3, cols = 4;
int (*matrix)[cols] = malloc(rows * sizeof(*matrix));

// Use as 2D array
for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        matrix[i][j] = i * cols + j;
    }
}

// Single free
free(matrix);
```

### 2. 2D Array - Array of Pointers
```c
// Allocate array of row pointers
int rows = 3, cols = 4;
int **matrix = (int**)malloc(rows * sizeof(int*));
if (matrix == NULL) return -1;

// Allocate each row
for (int i = 0; i < rows; i++) {
    matrix[i] = (int*)malloc(cols * sizeof(int));
    if (matrix[i] == NULL) {
        // Clean up on failure
        for (int j = 0; j < i; j++) {
            free(matrix[j]);
        }
        free(matrix);
        return -1;
    }
}

// Use matrix
for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        matrix[i][j] = i * cols + j;
    }
}

// Free in reverse order
for (int i = 0; i < rows; i++) {
    free(matrix[i]);
}
free(matrix);
```

### 3. Dynamic Structure with Dynamic Members
```c
typedef struct {
    char *name;
    int *scores;
    int num_scores;
} Student;

// Allocate student
Student *s = (Student*)malloc(sizeof(Student));
if (s == NULL) return -1;

// Allocate name
s->name = (char*)malloc(50 * sizeof(char));
if (s->name == NULL) {
    free(s);
    return -1;
}
strcpy(s->name, "Alice");

// Allocate scores array
s->num_scores = 5;
s->scores = (int*)calloc(s->num_scores, sizeof(int));
if (s->scores == NULL) {
    free(s->name);
    free(s);
    return -1;
}

// Use structure
for (int i = 0; i < s->num_scores; i++) {
    s->scores[i] = 85 + i;
}

// Free in reverse order of allocation
free(s->scores);
free(s->name);
free(s);
```

### 4. Flexible Array Member (C99)
```c
typedef struct {
    int size;
    int data[];  // Flexible array member
} FlexArray;

// Allocate structure with extra space for array
int n = 10;
FlexArray *arr = malloc(sizeof(FlexArray) + n * sizeof(int));
if (arr == NULL) return -1;

arr->size = n;
for (int i = 0; i < n; i++) {
    arr->data[i] = i * i;
}

// Single free for everything
free(arr);
```

## Memory Management Best Practices

### 1. Always Check Allocation Success
```c
void* safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL && size > 0) {
        fprintf(stderr, "Memory allocation failed for %zu bytes\n", size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}
```

### 2. Initialize After Allocation
```c
// Option 1: Use calloc for zero initialization
int *arr = (int*)calloc(10, sizeof(int));

// Option 2: Use memset after malloc
int *arr = (int*)malloc(10 * sizeof(int));
if (arr) {
    memset(arr, 0, 10 * sizeof(int));
}

// Option 3: Initialize immediately
int *arr = (int*)malloc(10 * sizeof(int));
if (arr) {
    for (int i = 0; i < 10; i++) {
        arr[i] = 0;
    }
}
```

### 3. Free Memory in Reverse Order
```c
// Allocation order: A -> B -> C
void *A = malloc(100);
void *B = malloc(200);
void *C = malloc(300);

// Free in reverse: C -> B -> A
free(C);
free(B);
free(A);
```

### 4. Set Pointers to NULL After Free
```c
int *ptr = malloc(sizeof(int));
// Use ptr...
free(ptr);
ptr = NULL;  // Prevents use-after-free
```

### 5. Create Wrapper Functions
```c
// Safe string duplication
char* safe_strdup(const char *src) {
    if (src == NULL) return NULL;
    
    size_t len = strlen(src) + 1;
    char *dst = malloc(len);
    if (dst) {
        memcpy(dst, src, len);
    }
    return dst;
}

// Safe array allocation
int* create_int_array(size_t size, int init_value) {
    int *arr = malloc(size * sizeof(int));
    if (arr) {
        for (size_t i = 0; i < size; i++) {
            arr[i] = init_value;
        }
    }
    return arr;
}
```

## Common Errors and Solutions

### 1. Memory Leaks
```c
// BAD: Memory leak
void leak_example() {
    int *data = malloc(100 * sizeof(int));
    // Missing free(data);
}  // Memory leaked!

// GOOD: Proper cleanup
void no_leak_example() {
    int *data = malloc(100 * sizeof(int));
    // Use data...
    free(data);
}
```

### 2. Use After Free
```c
// BAD: Use after free
int *ptr = malloc(sizeof(int));
free(ptr);
*ptr = 10;  // Undefined behavior!

// GOOD: Set to NULL after free
int *ptr = malloc(sizeof(int));
free(ptr);
ptr = NULL;
if (ptr != NULL) {  // This check prevents use
    *ptr = 10;
}
```

### 3. Double Free
```c
// BAD: Double free
int *ptr = malloc(sizeof(int));
free(ptr);
free(ptr);  // Undefined behavior!

// GOOD: Use a wrapper
void safe_free(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

int *ptr = malloc(sizeof(int));
safe_free((void**)&ptr);
safe_free((void**)&ptr);  // Safe, does nothing
```

### 4. Buffer Overflow
```c
// BAD: Writing beyond allocated memory
int *arr = malloc(5 * sizeof(int));
for (int i = 0; i <= 5; i++) {  // Bug: <= instead of <
    arr[i] = i;
}

// GOOD: Respect boundaries
int *arr = malloc(5 * sizeof(int));
for (int i = 0; i < 5; i++) {
    arr[i] = i;
}
```

## Memory Debugging Tips

### 1. Use Valgrind
```bash
gcc -g program.c -o program
valgrind --leak-check=full --show-leak-kinds=all ./program
```

### 2. Custom Memory Tracking
```c
#ifdef DEBUG
static size_t allocated_bytes = 0;
static size_t allocation_count = 0;

void* debug_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (ptr) {
        allocated_bytes += size;
        allocation_count++;
        printf("[ALLOC] %zu bytes at %s:%d (total: %zu)\n", 
               size, file, line, allocated_bytes);
    }
    return ptr;
}

#define malloc(size) debug_malloc(size, __FILE__, __LINE__)
#endif
```

## Quick Reference
| Function | Purpose | Returns | Initialization |
|----------|---------|---------|----------------|
| `malloc(size)` | Allocate bytes | Pointer or NULL | Uninitialized |
| `calloc(n, size)` | Allocate array | Pointer or NULL | Zero-initialized |
| `free(ptr)` | Deallocate | void | N/A |
| `sizeof(type)` | Get type size | size_t | N/A |

## Exam Tips
1. Always check if allocation succeeded (compare with NULL)
2. Match every malloc/calloc with exactly one free
3. Never use memory after free
4. Set pointers to NULL after freeing
5. Free in reverse order of allocation
6. Use calloc for zero-initialized memory
7. Calculate array sizes as `count * sizeof(element)`
8. Remember: sizeof(pointer) ≠ sizeof(pointed data)