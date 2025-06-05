#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Error handling macro
#define CHECK_ALLOC(ptr) do { \
    if ((ptr) == NULL) { \
        fprintf(stderr, "Memory allocation failed at %s:%d\n", __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

// Basic allocation examples
void basic_allocation_demo() {
    printf("\n=== Basic Memory Allocation Demo ===\n");
    
    // Single integer
    int *single_int = (int*)malloc(sizeof(int));
    CHECK_ALLOC(single_int);
    *single_int = 42;
    printf("Single integer: %d\n", *single_int);
    free(single_int);
    
    // Array of integers using malloc (uninitialized)
    int n = 5;
    int *arr_malloc = (int*)malloc(n * sizeof(int));
    CHECK_ALLOC(arr_malloc);
    printf("Malloc array (uninitialized): ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr_malloc[i]);  // Garbage values
    }
    printf("\n");
    
    // Initialize the array
    for (int i = 0; i < n; i++) {
        arr_malloc[i] = i * 10;
    }
    printf("Malloc array (initialized): ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr_malloc[i]);
    }
    printf("\n");
    free(arr_malloc);
    
    // Array using calloc (zero-initialized)
    int *arr_calloc = (int*)calloc(n, sizeof(int));
    CHECK_ALLOC(arr_calloc);
    printf("Calloc array (zero-initialized): ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr_calloc[i]);  // All zeros
    }
    printf("\n");
    free(arr_calloc);
}

// String allocation examples
void string_allocation_demo() {
    printf("\n=== String Allocation Demo ===\n");
    
    // Fixed size string buffer
    char *buffer = (char*)malloc(100 * sizeof(char));
    CHECK_ALLOC(buffer);
    
    strcpy(buffer, "Hello, Dynamic Memory!");
    printf("Buffer content: %s\n", buffer);
    printf("Buffer length: %zu\n", strlen(buffer));
    
    // String concatenation
    strcat(buffer, " This is C programming.");
    printf("After concatenation: %s\n", buffer);
    
    free(buffer);
    
    // Dynamic string duplication
    const char *original = "Original String";
    char *copy = (char*)malloc((strlen(original) + 1) * sizeof(char));
    CHECK_ALLOC(copy);
    strcpy(copy, original);
    
    printf("Original: %s (at %p)\n", original, (void*)original);
    printf("Copy: %s (at %p)\n", copy, (void*)copy);
    
    free(copy);
    
    // Array of strings
    int num_strings = 3;
    char **string_array = (char**)malloc(num_strings * sizeof(char*));
    CHECK_ALLOC(string_array);
    
    const char *words[] = {"First", "Second", "Third"};
    for (int i = 0; i < num_strings; i++) {
        string_array[i] = (char*)malloc((strlen(words[i]) + 1) * sizeof(char));
        CHECK_ALLOC(string_array[i]);
        strcpy(string_array[i], words[i]);
    }
    
    printf("String array:\n");
    for (int i = 0; i < num_strings; i++) {
        printf("  [%d] %s\n", i, string_array[i]);
    }
    
    // Free string array
    for (int i = 0; i < num_strings; i++) {
        free(string_array[i]);
    }
    free(string_array);
}

// Structure allocation
typedef struct {
    int id;
    char *name;
    double *grades;
    int num_grades;
} Student;

Student* create_student(int id, const char *name, int num_grades) {
    Student *s = (Student*)malloc(sizeof(Student));
    CHECK_ALLOC(s);
    
    s->id = id;
    
    // Allocate and copy name
    s->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    CHECK_ALLOC(s->name);
    strcpy(s->name, name);
    
    // Allocate grades array
    s->num_grades = num_grades;
    s->grades = (double*)calloc(num_grades, sizeof(double));
    CHECK_ALLOC(s->grades);
    
    return s;
}

void free_student(Student *s) {
    if (s == NULL) return;
    
    free(s->grades);
    free(s->name);
    free(s);
}

void structure_allocation_demo() {
    printf("\n=== Structure Allocation Demo ===\n");
    
    // Create a single student
    Student *student = create_student(12345, "John Doe", 5);
    
    // Set grades
    for (int i = 0; i < student->num_grades; i++) {
        student->grades[i] = 85.0 + i * 2;
    }
    
    // Display student info
    printf("Student ID: %d\n", student->id);
    printf("Name: %s\n", student->name);
    printf("Grades: ");
    for (int i = 0; i < student->num_grades; i++) {
        printf("%.1f ", student->grades[i]);
    }
    printf("\n");
    
    free_student(student);
    
    // Array of students
    int num_students = 3;
    Student **class = (Student**)malloc(num_students * sizeof(Student*));
    CHECK_ALLOC(class);
    
    const char *names[] = {"Alice", "Bob", "Charlie"};
    for (int i = 0; i < num_students; i++) {
        class[i] = create_student(1000 + i, names[i], 3);
        for (int j = 0; j < 3; j++) {
            class[i]->grades[j] = 70.0 + i * 5 + j * 3;
        }
    }
    
    printf("\nClass roster:\n");
    for (int i = 0; i < num_students; i++) {
        printf("  %s (ID: %d): ", class[i]->name, class[i]->id);
        for (int j = 0; j < class[i]->num_grades; j++) {
            printf("%.1f ", class[i]->grades[j]);
        }
        printf("\n");
    }
    
    // Free all students
    for (int i = 0; i < num_students; i++) {
        free_student(class[i]);
    }
    free(class);
}

// 2D array allocation methods
void matrix_allocation_demo() {
    printf("\n=== 2D Array Allocation Demo ===\n");
    
    int rows = 3, cols = 4;
    
    // Method 1: Single block allocation (C99 VLA style)
    int (*matrix1)[cols] = malloc(rows * sizeof(*matrix1));
    CHECK_ALLOC(matrix1);
    
    // Initialize
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix1[i][j] = i * cols + j;
        }
    }
    
    printf("Method 1 (single block):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", matrix1[i][j]);
        }
        printf("\n");
    }
    
    free(matrix1);
    
    // Method 2: Array of pointers
    int **matrix2 = (int**)malloc(rows * sizeof(int*));
    CHECK_ALLOC(matrix2);
    
    for (int i = 0; i < rows; i++) {
        matrix2[i] = (int*)malloc(cols * sizeof(int));
        CHECK_ALLOC(matrix2[i]);
    }
    
    // Initialize
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix2[i][j] = (i + 1) * (j + 1);
        }
    }
    
    printf("\nMethod 2 (array of pointers):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", matrix2[i][j]);
        }
        printf("\n");
    }
    
    // Free matrix2
    for (int i = 0; i < rows; i++) {
        free(matrix2[i]);
    }
    free(matrix2);
    
    // Method 3: Single allocation with pointer arithmetic
    int *matrix3 = (int*)malloc(rows * cols * sizeof(int));
    CHECK_ALLOC(matrix3);
    
    // Access using: matrix3[i * cols + j]
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix3[i * cols + j] = i + j;
        }
    }
    
    printf("\nMethod 3 (single allocation):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", matrix3[i * cols + j]);
        }
        printf("\n");
    }
    
    free(matrix3);
}

// Dynamic growing array
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} DynamicArray;

DynamicArray* array_create(size_t initial_capacity) {
    DynamicArray *arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    CHECK_ALLOC(arr);
    
    arr->data = (int*)malloc(initial_capacity * sizeof(int));
    CHECK_ALLOC(arr->data);
    
    arr->size = 0;
    arr->capacity = initial_capacity;
    
    return arr;
}

void array_push(DynamicArray *arr, int value) {
    if (arr->size >= arr->capacity) {
        // Double the capacity
        size_t new_capacity = arr->capacity * 2;
        int *new_data = (int*)realloc(arr->data, new_capacity * sizeof(int));
        CHECK_ALLOC(new_data);
        
        arr->data = new_data;
        arr->capacity = new_capacity;
        
        printf("Array expanded to capacity %zu\n", new_capacity);
    }
    
    arr->data[arr->size++] = value;
}

void array_free(DynamicArray *arr) {
    if (arr) {
        free(arr->data);
        free(arr);
    }
}

void dynamic_array_demo() {
    printf("\n=== Dynamic Growing Array Demo ===\n");
    
    DynamicArray *arr = array_create(4);
    
    // Add elements (will trigger reallocation)
    for (int i = 0; i < 10; i++) {
        array_push(arr, i * i);
        printf("Added %d, size=%zu, capacity=%zu\n", 
               i * i, arr->size, arr->capacity);
    }
    
    printf("Final array: ");
    for (size_t i = 0; i < arr->size; i++) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
    
    array_free(arr);
}

// Memory pool allocator
typedef struct {
    char *pool;
    size_t pool_size;
    size_t used;
} MemoryPool;

MemoryPool* pool_create(size_t size) {
    MemoryPool *pool = (MemoryPool*)malloc(sizeof(MemoryPool));
    CHECK_ALLOC(pool);
    
    pool->pool = (char*)malloc(size);
    CHECK_ALLOC(pool->pool);
    
    pool->pool_size = size;
    pool->used = 0;
    
    return pool;
}

void* pool_alloc(MemoryPool *pool, size_t size) {
    // Align to 8 bytes
    size = (size + 7) & ~7;
    
    if (pool->used + size > pool->pool_size) {
        return NULL;  // Pool exhausted
    }
    
    void *ptr = pool->pool + pool->used;
    pool->used += size;
    
    return ptr;
}

void pool_reset(MemoryPool *pool) {
    pool->used = 0;
}

void pool_destroy(MemoryPool *pool) {
    if (pool) {
        free(pool->pool);
        free(pool);
    }
}

void memory_pool_demo() {
    printf("\n=== Memory Pool Demo ===\n");
    
    MemoryPool *pool = pool_create(1024);
    
    // Allocate from pool
    int *arr1 = (int*)pool_alloc(pool, 10 * sizeof(int));
    double *arr2 = (double*)pool_alloc(pool, 5 * sizeof(double));
    char *str = (char*)pool_alloc(pool, 50);
    
    if (arr1 && arr2 && str) {
        // Use allocated memory
        for (int i = 0; i < 10; i++) {
            arr1[i] = i;
        }
        for (int i = 0; i < 5; i++) {
            arr2[i] = i * 3.14;
        }
        strcpy(str, "Memory pool allocation");
        
        printf("Pool allocated successfully\n");
        printf("Used: %zu / %zu bytes\n", pool->used, pool->pool_size);
    }
    
    // No individual free needed!
    pool_destroy(pool);
}

// Demonstrate common errors (for educational purposes)
void common_errors_demo() {
    printf("\n=== Common Memory Errors (Educational) ===\n");
    
    // Error 1: Forgetting to check allocation
    printf("1. Always check allocation result\n");
    int *ptr = (int*)malloc(sizeof(int));
    if (ptr != NULL) {
        *ptr = 42;
        printf("   Value: %d\n", *ptr);
        free(ptr);
    }
    
    // Error 2: Memory leak prevention
    printf("2. Prevent memory leaks\n");
    for (int i = 0; i < 3; i++) {
        int *leak = (int*)malloc(sizeof(int));
        CHECK_ALLOC(leak);
        *leak = i;
        printf("   Allocated %d\n", *leak);
        free(leak);  // Important!
    }
    
    // Error 3: Correct size calculation
    printf("3. Correct size calculation\n");
    int n = 5;
    int *arr = (int*)malloc(n * sizeof(int));  // Correct
    // NOT: malloc(n) which would be too small
    CHECK_ALLOC(arr);
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
    free(arr);
    
    printf("All error demos completed safely\n");
}

int main() {
    printf("DYNAMIC MEMORY ALLOCATION EXAMPLES\n");
    printf("==================================\n");
    
    basic_allocation_demo();
    string_allocation_demo();
    structure_allocation_demo();
    matrix_allocation_demo();
    dynamic_array_demo();
    memory_pool_demo();
    common_errors_demo();
    
    printf("\nAll demos completed successfully!\n");
    
    return 0;
}