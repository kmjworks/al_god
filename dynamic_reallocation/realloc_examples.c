#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Helper macro for safe reallocation
#define SAFE_REALLOC(ptr, size) safe_realloc((void**)&(ptr), size)

int safe_realloc(void **ptr, size_t size) {
    void *new_ptr = realloc(*ptr, size);
    if (new_ptr == NULL && size > 0) {
        return 0;  // Failed
    }
    *ptr = new_ptr;
    return 1;  // Success
}

// Basic realloc demonstration
void basic_realloc_demo() {
    printf("\n=== Basic Realloc Demo ===\n");
    
    // Start with small array
    int initial_size = 5;
    int *arr = (int*)malloc(initial_size * sizeof(int));
    if (!arr) {
        printf("Initial allocation failed\n");
        return;
    }
    
    // Fill initial array
    printf("Initial array (size %d): ", initial_size);
    for (int i = 0; i < initial_size; i++) {
        arr[i] = i * 10;
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // Grow the array
    int new_size = 10;
    printf("\nGrowing array to size %d...\n", new_size);
    int *temp = (int*)realloc(arr, new_size * sizeof(int));
    
    if (temp == NULL) {
        printf("Reallocation failed\n");
        free(arr);
        return;
    }
    
    arr = temp;
    printf("Reallocation successful. Pointer changed: %s\n", 
           (void*)temp != (void*)arr ? "Yes" : "No");
    
    // Initialize new elements
    for (int i = initial_size; i < new_size; i++) {
        arr[i] = i * 10;
    }
    
    // Display grown array
    printf("Grown array: ");
    for (int i = 0; i < new_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // Shrink the array
    int shrink_size = 3;
    printf("\nShrinking array to size %d...\n", shrink_size);
    temp = (int*)realloc(arr, shrink_size * sizeof(int));
    
    if (temp != NULL) {
        arr = temp;
        printf("Shrunk array: ");
        for (int i = 0; i < shrink_size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
    
    free(arr);
}

// Dynamic string builder
typedef struct {
    char *str;
    size_t length;
    size_t capacity;
} StringBuilder;

StringBuilder* sb_create(size_t initial_capacity) {
    StringBuilder *sb = (StringBuilder*)malloc(sizeof(StringBuilder));
    if (!sb) return NULL;
    
    sb->capacity = initial_capacity > 0 ? initial_capacity : 16;
    sb->length = 0;
    sb->str = (char*)malloc(sb->capacity);
    
    if (!sb->str) {
        free(sb);
        return NULL;
    }
    
    sb->str[0] = '\0';
    return sb;
}

int sb_append(StringBuilder *sb, const char *text) {
    if (!sb || !text) return -1;
    
    size_t text_len = strlen(text);
    size_t required = sb->length + text_len + 1;
    
    // Check if we need to grow
    if (required > sb->capacity) {
        size_t new_capacity = sb->capacity * 2;
        while (new_capacity < required) {
            new_capacity *= 2;
        }
        
        printf("  StringBuilder: Growing capacity from %zu to %zu\n", 
               sb->capacity, new_capacity);
        
        char *new_str = (char*)realloc(sb->str, new_capacity);
        if (!new_str) {
            return -1;
        }
        
        sb->str = new_str;
        sb->capacity = new_capacity;
    }
    
    strcpy(sb->str + sb->length, text);
    sb->length += text_len;
    
    return 0;
}

void sb_free(StringBuilder *sb) {
    if (sb) {
        free(sb->str);
        free(sb);
    }
}

void string_builder_demo() {
    printf("\n=== String Builder Demo ===\n");
    
    StringBuilder *sb = sb_create(8);
    if (!sb) {
        printf("Failed to create string builder\n");
        return;
    }
    
    const char *words[] = {
        "Hello", " ", "World", "! ", 
        "This ", "is ", "a ", "dynamic ", "string."
    };
    
    printf("Building string with initial capacity: %zu\n", sb->capacity);
    
    for (int i = 0; i < 9; i++) {
        sb_append(sb, words[i]);
        printf("  After '%s': len=%zu, cap=%zu\n", 
               words[i], sb->length, sb->capacity);
    }
    
    printf("\nFinal string: '%s'\n", sb->str);
    printf("Final length: %zu, capacity: %zu\n", sb->length, sb->capacity);
    
    sb_free(sb);
}

// Dynamic array with statistics
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
    size_t realloc_count;
    size_t total_allocated;
} DynamicArray;

DynamicArray* da_create() {
    DynamicArray *da = (DynamicArray*)calloc(1, sizeof(DynamicArray));
    if (!da) return NULL;
    
    da->capacity = 4;  // Start small to demonstrate growth
    da->data = (int*)malloc(da->capacity * sizeof(int));
    if (!da->data) {
        free(da);
        return NULL;
    }
    
    da->total_allocated = da->capacity * sizeof(int);
    return da;
}

int da_push(DynamicArray *da, int value) {
    if (da->size >= da->capacity) {
        size_t new_capacity = da->capacity * 2;
        
        int *new_data = (int*)realloc(da->data, new_capacity * sizeof(int));
        if (!new_data) return -1;
        
        da->data = new_data;
        da->capacity = new_capacity;
        da->realloc_count++;
        da->total_allocated += (new_capacity - da->capacity/2) * sizeof(int);
        
        printf("  Realloc #%zu: capacity %zu -> %zu\n", 
               da->realloc_count, da->capacity/2, da->capacity);
    }
    
    da->data[da->size++] = value;
    return 0;
}

void da_stats(DynamicArray *da) {
    printf("\nDynamic Array Statistics:\n");
    printf("  Elements: %zu\n", da->size);
    printf("  Capacity: %zu\n", da->capacity);
    printf("  Reallocations: %zu\n", da->realloc_count);
    printf("  Memory efficiency: %.1f%%\n", 
           (da->size * 100.0) / da->capacity);
    printf("  Total allocated: %zu bytes\n", da->total_allocated);
}

void da_free(DynamicArray *da) {
    if (da) {
        free(da->data);
        free(da);
    }
}

void dynamic_array_demo() {
    printf("\n=== Dynamic Array Growth Demo ===\n");
    
    DynamicArray *da = da_create();
    if (!da) return;
    
    printf("Adding 20 elements to demonstrate growth pattern:\n");
    
    for (int i = 0; i < 20; i++) {
        da_push(da, i * i);
    }
    
    da_stats(da);
    
    printf("\nArray contents: ");
    for (size_t i = 0; i < da->size; i++) {
        printf("%d ", da->data[i]);
    }
    printf("\n");
    
    da_free(da);
}

// Memory pool with reallocation
typedef struct {
    void *memory;
    size_t size;
    size_t used;
    size_t realloc_count;
} MemoryPool;

MemoryPool* pool_create(size_t initial_size) {
    MemoryPool *pool = (MemoryPool*)calloc(1, sizeof(MemoryPool));
    if (!pool) return NULL;
    
    pool->size = initial_size;
    pool->memory = malloc(initial_size);
    if (!pool->memory) {
        free(pool);
        return NULL;
    }
    
    return pool;
}

void* pool_alloc(MemoryPool *pool, size_t bytes) {
    // Align to 8 bytes
    bytes = (bytes + 7) & ~7;
    
    if (pool->used + bytes > pool->size) {
        size_t new_size = pool->size * 2;
        while (new_size < pool->used + bytes) {
            new_size *= 2;
        }
        
        void *new_memory = realloc(pool->memory, new_size);
        if (!new_memory) return NULL;
        
        printf("  Pool realloc: %zu -> %zu bytes\n", pool->size, new_size);
        pool->memory = new_memory;
        pool->size = new_size;
        pool->realloc_count++;
    }
    
    void *ptr = (char*)pool->memory + pool->used;
    pool->used += bytes;
    
    return ptr;
}

void pool_free(MemoryPool *pool) {
    if (pool) {
        free(pool->memory);
        free(pool);
    }
}

void memory_pool_demo() {
    printf("\n=== Memory Pool Demo ===\n");
    
    MemoryPool *pool = pool_create(64);  // Start small
    if (!pool) return;
    
    printf("Initial pool size: %zu bytes\n", pool->size);
    
    // Allocate various sized chunks
    int *arr1 = (int*)pool_alloc(pool, 10 * sizeof(int));
    double *arr2 = (double*)pool_alloc(pool, 5 * sizeof(double));
    char *str = (char*)pool_alloc(pool, 100);
    
    if (arr1 && arr2 && str) {
        // Use allocations
        for (int i = 0; i < 10; i++) arr1[i] = i;
        for (int i = 0; i < 5; i++) arr2[i] = i * 3.14;
        strcpy(str, "Memory pool allocation test");
        
        printf("Successfully allocated from pool\n");
        printf("Pool usage: %zu / %zu bytes\n", pool->used, pool->size);
        printf("Pool reallocations: %zu\n", pool->realloc_count);
    }
    
    pool_free(pool);
}

// Demonstrate realloc edge cases
void edge_cases_demo() {
    printf("\n=== Realloc Edge Cases Demo ===\n");
    
    // Case 1: realloc with NULL pointer
    printf("\n1. realloc(NULL, size) - acts like malloc:\n");
    int *ptr1 = (int*)realloc(NULL, 5 * sizeof(int));
    if (ptr1) {
        printf("   Success: allocated %zu bytes\n", 5 * sizeof(int));
        free(ptr1);
    }
    
    // Case 2: realloc to size 0
    printf("\n2. realloc(ptr, 0) - acts like free:\n");
    int *ptr2 = (int*)malloc(10 * sizeof(int));
    printf("   Original pointer: %p\n", (void*)ptr2);
    ptr2 = (int*)realloc(ptr2, 0);
    printf("   After realloc(ptr, 0): %p\n", (void*)ptr2);
    
    // Case 3: realloc to same size
    printf("\n3. realloc to same size:\n");
    int *ptr3 = (int*)malloc(10 * sizeof(int));
    void *original = ptr3;
    ptr3 = (int*)realloc(ptr3, 10 * sizeof(int));
    printf("   Pointer changed: %s\n", ptr3 != original ? "Yes" : "No");
    free(ptr3);
    
    // Case 4: Shrinking significantly
    printf("\n4. Significant shrinking:\n");
    size_t large_size = 1000000;  // 1MB of ints
    size_t small_size = 10;
    int *ptr4 = (int*)malloc(large_size * sizeof(int));
    if (ptr4) {
        printf("   Allocated %zu bytes\n", large_size * sizeof(int));
        int *shrunk = (int*)realloc(ptr4, small_size * sizeof(int));
        if (shrunk) {
            printf("   Shrunk to %zu bytes\n", small_size * sizeof(int));
            printf("   Pointer changed: %s\n", shrunk != ptr4 ? "Yes" : "No");
            free(shrunk);
        } else {
            free(ptr4);
        }
    }
}

// Demonstrate proper error handling
void error_handling_demo() {
    printf("\n=== Realloc Error Handling Demo ===\n");
    
    // Allocate initial array
    size_t size = 10;
    int *data = (int*)calloc(size, sizeof(int));
    if (!data) return;
    
    // Fill with important data
    for (size_t i = 0; i < size; i++) {
        data[i] = i * 100;
    }
    
    printf("Original data: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    
    // Try to allocate huge amount (likely to fail)
    size_t huge_size = SIZE_MAX / 2;
    printf("\nTrying to realloc to huge size...\n");
    
    int *temp = (int*)realloc(data, huge_size);
    if (temp == NULL) {
        printf("Realloc failed (as expected)\n");
        printf("Original data preserved: ");
        for (size_t i = 0; i < size; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    } else {
        // Unlikely but possible
        data = temp;
        printf("Surprisingly, huge realloc succeeded!\n");
    }
    
    free(data);
}

// Performance comparison: realloc strategies
void performance_comparison_demo() {
    printf("\n=== Realloc Strategy Comparison ===\n");
    
    const int iterations = 1000;
    
    // Strategy 1: Linear growth
    printf("\n1. Linear growth (size + 10):\n");
    int *linear = NULL;
    size_t linear_cap = 0;
    size_t linear_reallocs = 0;
    
    for (int i = 0; i < iterations; i++) {
        if (i >= linear_cap) {
            linear_cap += 10;
            int *temp = (int*)realloc(linear, linear_cap * sizeof(int));
            if (!temp) break;
            linear = temp;
            linear_reallocs++;
        }
        linear[i] = i;
    }
    
    printf("   Final capacity: %zu\n", linear_cap);
    printf("   Reallocations: %zu\n", linear_reallocs);
    printf("   Wasted space: %zu elements\n", linear_cap - iterations);
    free(linear);
    
    // Strategy 2: Exponential growth
    printf("\n2. Exponential growth (size * 2):\n");
    int *exponential = NULL;
    size_t exp_cap = 0;
    size_t exp_reallocs = 0;
    
    for (int i = 0; i < iterations; i++) {
        if (i >= exp_cap) {
            exp_cap = exp_cap == 0 ? 1 : exp_cap * 2;
            int *temp = (int*)realloc(exponential, exp_cap * sizeof(int));
            if (!temp) break;
            exponential = temp;
            exp_reallocs++;
        }
        exponential[i] = i;
    }
    
    printf("   Final capacity: %zu\n", exp_cap);
    printf("   Reallocations: %zu\n", exp_reallocs);
    printf("   Wasted space: %zu elements\n", exp_cap - iterations);
    free(exponential);
}

int main() {
    printf("DYNAMIC MEMORY REALLOCATION EXAMPLES\n");
    printf("====================================\n");
    
    basic_realloc_demo();
    string_builder_demo();
    dynamic_array_demo();
    memory_pool_demo();
    edge_cases_demo();
    error_handling_demo();
    performance_comparison_demo();
    
    printf("\nAll demos completed successfully!\n");
    
    return 0;
}