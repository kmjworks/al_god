# Dynamic Memory Reallocation Cheat Sheet - C Programming

## realloc() Function Overview

### Function Signature
```c
void* realloc(void* ptr, size_t new_size);
```

### Parameters
- `ptr`: Pointer to previously allocated memory block (or NULL)
- `new_size`: New size in bytes for the memory block

### Return Value
- Success: Pointer to reallocated memory (may be different from original)
- Failure: NULL (original memory remains unchanged)

### Key Behaviors
1. If `ptr` is NULL, behaves like `malloc(new_size)`
2. If `new_size` is 0, behaves like `free(ptr)` and returns NULL
3. May move memory to a new location if needed
4. Preserves existing data up to minimum of old and new sizes
5. New memory (if size increased) is uninitialized

## Basic Usage

### 1. Growing an Array
```c
// Initial allocation
int capacity = 5;
int *arr = (int*)malloc(capacity * sizeof(int));
if (arr == NULL) {
    return -1;
}

// Fill initial array
for (int i = 0; i < capacity; i++) {
    arr[i] = i * 10;
}

// Grow the array
int new_capacity = 10;
int *temp = (int*)realloc(arr, new_capacity * sizeof(int));
if (temp == NULL) {
    // realloc failed, original array still valid
    free(arr);
    return -1;
}
arr = temp;  // Update pointer
capacity = new_capacity;

// Initialize new elements
for (int i = 5; i < capacity; i++) {
    arr[i] = i * 10;
}
```

### 2. Shrinking an Array
```c
// Start with larger array
int size = 100;
double *data = (double*)calloc(size, sizeof(double));
if (data == NULL) return -1;

// Use only first 20 elements
int actual_size = 20;

// Shrink to save memory
double *temp = (double*)realloc(data, actual_size * sizeof(double));
if (temp != NULL) {
    data = temp;  // Shrinking usually succeeds
    size = actual_size;
}
// If realloc fails, continue with original allocation
```

### 3. Safe Reallocation Pattern
```c
// ALWAYS use a temporary pointer
void* safe_realloc(void *ptr, size_t new_size) {
    void *new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL && new_size > 0) {
        // Reallocation failed, original pointer still valid
        return NULL;
    }
    return new_ptr;
}

// Usage
int *arr = (int*)malloc(10 * sizeof(int));
int *temp = (int*)safe_realloc(arr, 20 * sizeof(int));
if (temp == NULL) {
    // Handle error, arr is still valid
    free(arr);
    return -1;
}
arr = temp;  // Success, update pointer
```

## Advanced Reallocation Patterns

### 1. Dynamic String Builder
```c
typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} StringBuilder;

StringBuilder* sb_create() {
    StringBuilder *sb = malloc(sizeof(StringBuilder));
    if (!sb) return NULL;
    
    sb->capacity = 16;  // Initial capacity
    sb->length = 0;
    sb->data = malloc(sb->capacity);
    if (!sb->data) {
        free(sb);
        return NULL;
    }
    sb->data[0] = '\0';
    
    return sb;
}

int sb_append(StringBuilder *sb, const char *str) {
    size_t str_len = strlen(str);
    size_t required = sb->length + str_len + 1;
    
    if (required > sb->capacity) {
        // Need to grow
        size_t new_capacity = sb->capacity * 2;
        while (new_capacity < required) {
            new_capacity *= 2;
        }
        
        char *new_data = realloc(sb->data, new_capacity);
        if (!new_data) return -1;
        
        sb->data = new_data;
        sb->capacity = new_capacity;
    }
    
    strcpy(sb->data + sb->length, str);
    sb->length += str_len;
    
    return 0;
}

void sb_free(StringBuilder *sb) {
    if (sb) {
        free(sb->data);
        free(sb);
    }
}
```

### 2. Growing Array with Exponential Growth
```c
typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} IntVector;

IntVector* vector_create() {
    IntVector *vec = malloc(sizeof(IntVector));
    if (!vec) return NULL;
    
    vec->capacity = 4;
    vec->count = 0;
    vec->items = malloc(vec->capacity * sizeof(int));
    if (!vec->items) {
        free(vec);
        return NULL;
    }
    
    return vec;
}

int vector_push(IntVector *vec, int value) {
    if (vec->count >= vec->capacity) {
        // Grow by factor of 1.5-2x
        size_t new_capacity = vec->capacity + vec->capacity / 2;
        if (new_capacity < vec->capacity + 1) {
            new_capacity = vec->capacity + 1;  // Overflow protection
        }
        
        int *new_items = realloc(vec->items, new_capacity * sizeof(int));
        if (!new_items) return -1;
        
        vec->items = new_items;
        vec->capacity = new_capacity;
    }
    
    vec->items[vec->count++] = value;
    return 0;
}

void vector_shrink_to_fit(IntVector *vec) {
    if (vec->count < vec->capacity) {
        int *new_items = realloc(vec->items, vec->count * sizeof(int));
        if (new_items || vec->count == 0) {
            vec->items = new_items;
            vec->capacity = vec->count;
        }
    }
}
```

### 3. Memory Pool with Reallocation
```c
typedef struct {
    void *memory;
    size_t size;
    size_t used;
    size_t alignment;
} MemPool;

void* mempool_alloc(MemPool *pool, size_t bytes) {
    // Align allocation
    size_t aligned_bytes = (bytes + pool->alignment - 1) & ~(pool->alignment - 1);
    
    if (pool->used + aligned_bytes > pool->size) {
        // Need to grow pool
        size_t new_size = pool->size * 2;
        while (new_size < pool->used + aligned_bytes) {
            new_size *= 2;
        }
        
        void *new_memory = realloc(pool->memory, new_size);
        if (!new_memory) return NULL;
        
        pool->memory = new_memory;
        pool->size = new_size;
    }
    
    void *ptr = (char*)pool->memory + pool->used;
    pool->used += aligned_bytes;
    
    return ptr;
}
```

### 4. Circular Buffer with Dynamic Sizing
```c
typedef struct {
    char *buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} CircularBuffer;

int cb_resize(CircularBuffer *cb, size_t new_capacity) {
    if (new_capacity < cb->count) return -1;  // Can't shrink below current count
    
    char *new_buffer = malloc(new_capacity);
    if (!new_buffer) return -1;
    
    // Copy existing data
    size_t copied = 0;
    if (cb->count > 0) {
        if (cb->head < cb->tail) {
            memcpy(new_buffer, cb->buffer + cb->head, cb->count);
        } else {
            // Wrapped around
            size_t first_part = cb->capacity - cb->head;
            memcpy(new_buffer, cb->buffer + cb->head, first_part);
            memcpy(new_buffer + first_part, cb->buffer, cb->tail);
        }
    }
    
    free(cb->buffer);
    cb->buffer = new_buffer;
    cb->capacity = new_capacity;
    cb->head = 0;
    cb->tail = cb->count;
    
    return 0;
}
```

## Common Pitfalls and Solutions

### 1. Losing Original Pointer
```c
// BAD: Original pointer lost if realloc fails
int *arr = malloc(10 * sizeof(int));
arr = realloc(arr, 20 * sizeof(int));  // If fails, memory leak!

// GOOD: Use temporary pointer
int *arr = malloc(10 * sizeof(int));
int *temp = realloc(arr, 20 * sizeof(int));
if (temp == NULL) {
    // Handle error, arr still valid
    free(arr);
    return -1;
}
arr = temp;
```

### 2. Assuming Pointer Remains Same
```c
// BAD: Storing pointers into reallocated array
int *arr = malloc(10 * sizeof(int));
int *ptr_to_element = &arr[5];

arr = realloc(arr, 20 * sizeof(int));  // arr might move!
// ptr_to_element is now invalid!

// GOOD: Store indices instead
size_t element_index = 5;
arr = realloc(arr, 20 * sizeof(int));
int *ptr_to_element = &arr[element_index];  // Recalculate after realloc
```

### 3. Not Initializing New Memory
```c
// BAD: Assuming new memory is zero
int *arr = calloc(5, sizeof(int));  // Zero-initialized
int *temp = realloc(arr, 10 * sizeof(int));
if (temp) {
    arr = temp;
    // arr[5] through arr[9] are NOT zero!
}

// GOOD: Initialize new portion
int old_size = 5;
int new_size = 10;
int *arr = calloc(old_size, sizeof(int));
int *temp = realloc(arr, new_size * sizeof(int));
if (temp) {
    arr = temp;
    // Initialize new elements
    for (int i = old_size; i < new_size; i++) {
        arr[i] = 0;
    }
}
```

### 4. Reallocating NULL vs Zero Size
```c
// Special cases
void *ptr = NULL;
ptr = realloc(ptr, 100);  // Same as malloc(100)

void *ptr2 = malloc(100);
ptr2 = realloc(ptr2, 0);  // Same as free(ptr2), returns NULL

// Portable zero-size handling
void *safe_realloc_zero(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    return realloc(ptr, size);
}
```

## Performance Considerations

### 1. Growth Strategies
```c
// Linear growth - O(n²) total reallocation cost
size = size + CONSTANT;

// Exponential growth - O(n) amortized cost
size = size * GROWTH_FACTOR;  // Common: 1.5 or 2

// Example implementation
size_t calculate_new_capacity(size_t current, size_t required) {
    size_t new_capacity = current;
    
    // Exponential growth
    while (new_capacity < required) {
        new_capacity = new_capacity + new_capacity / 2;  // 1.5x growth
        
        // Overflow protection
        if (new_capacity < current) {
            return SIZE_MAX;
        }
    }
    
    return new_capacity;
}
```

### 2. Shrinking Strategy
```c
// Don't shrink too aggressively
typedef struct {
    void *data;
    size_t size;
    size_t capacity;
} Buffer;

void buffer_maybe_shrink(Buffer *buf) {
    // Only shrink if using less than 25% of capacity
    if (buf->size < buf->capacity / 4 && buf->capacity > 16) {
        size_t new_capacity = buf->capacity / 2;
        void *new_data = realloc(buf->data, new_capacity);
        if (new_data) {
            buf->data = new_data;
            buf->capacity = new_capacity;
        }
    }
}
```

## Memory Debugging with realloc

### Custom Tracking Wrapper
```c
typedef struct AllocInfo {
    void *ptr;
    size_t size;
    const char *file;
    int line;
    struct AllocInfo *next;
} AllocInfo;

static AllocInfo *alloc_list = NULL;

void* debug_realloc(void *ptr, size_t new_size, const char *file, int line) {
    void *new_ptr = realloc(ptr, new_size);
    
    if (new_ptr || new_size == 0) {
        // Update tracking info
        AllocInfo *info = alloc_list;
        while (info) {
            if (info->ptr == ptr) {
                info->ptr = new_ptr;
                info->size = new_size;
                info->file = file;
                info->line = line;
                break;
            }
            info = info->next;
        }
        
        if (!info && new_ptr) {
            // New allocation (ptr was NULL)
            info = malloc(sizeof(AllocInfo));
            info->ptr = new_ptr;
            info->size = new_size;
            info->file = file;
            info->line = line;
            info->next = alloc_list;
            alloc_list = info;
        }
    }
    
    return new_ptr;
}

#define realloc(p, s) debug_realloc(p, s, __FILE__, __LINE__)
```

## Best Practices

1. **Always use a temporary pointer** for realloc return value
2. **Check for NULL** before updating original pointer
3. **Initialize new memory** when growing
4. **Track both size and capacity** for dynamic arrays
5. **Use exponential growth** for better performance
6. **Don't shrink too aggressively** to avoid thrashing
7. **Consider alignment** for performance
8. **Free the original pointer** if realloc fails and you're done

## Quick Reference

| Scenario | Code | Result |
|----------|------|--------|
| First allocation | `realloc(NULL, size)` | Same as `malloc(size)` |
| Free memory | `realloc(ptr, 0)` | Same as `free(ptr)` |
| Grow array | `temp = realloc(arr, new_size)` | May move memory |
| Shrink array | `temp = realloc(arr, smaller_size)` | Usually succeeds |
| Same size | `realloc(ptr, same_size)` | May return same ptr |

## Exam Tips
1. realloc can return a different pointer - never do `ptr = realloc(ptr, size)`
2. Original memory is preserved up to minimum of old and new sizes
3. New memory (when growing) is uninitialized
4. realloc(NULL, size) equals malloc(size)
5. realloc(ptr, 0) equals free(ptr) and returns NULL
6. Always check return value before updating pointer
7. Memory may be moved even when shrinking
8. Use temporary pointer to avoid losing original on failure