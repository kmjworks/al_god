
1. **generic_bubble_sort.c** - Type-agnostic sorting using void pointers
2. **generic_binary_tree.c** - Generic BST with any data type
3. **avl_tree.c** - Self-balancing tree with rotations
4. **generic_linked_list.c** - Doubly linked list with functional operations
5. **tricky_pointer_examples.c** - Complex pointer manipulations
6. **callback_examples.c** - Function pointers and event systems
7. **memory_mapped_io.c** - mmap() file operations
8. **custom_memory_allocator.c** - Custom malloc/free implementation

## Key Concepts to Remember:

### 1. Generic Programming in C
- Use `void*` for type-agnostic data
- Pass size with `size_t` parameter
- Use function pointers for comparisons: `int (*compare)(const void*, const void*)`
- Memory operations: `memcpy()`, `memmove()`, `memset()`

### 2. Function Pointers
```c
// Declaration
return_type (*pointer_name)(param_types);

// Array of function pointers
int (*operations[4])(int, int) = {add, sub, mul, div};

// Callback with context
typedef struct {
    void (*callback)(void *context, int value);
    void *context;
} CallbackWithContext;
```

### 3. Advanced Pointer Tricks
- Double pointers for 2D arrays: `int **matrix`
- Pointer arithmetic: `(char*)ptr + offset`
- Array of pointers vs pointer to array
- Tagged pointers (using LSB for flags)
- `offsetof()` macro for struct member offsets

### 4. Tree Balancing (AVL)
- Balance factor = height(left) - height(right)
- Four rotation cases: LL, RR, LR, RL
- Maintain balance after insertion/deletion

### 5. Memory Management
- Custom allocator needs: header, free list, coalescing
- Memory pools for fixed-size allocations
- mmap() for file I/O and shared memory
- Copy-on-write with MAP_PRIVATE

### 6. Common Patterns
- Circular buffers
- Event systems with callbacks
- State machines with function pointers
- Generic iterators
- Middleware chains

### 7. Important Headers
```c
#include <stddef.h>  // size_t, offsetof
#include <stdint.h>  // uintptr_t
#include <sys/mman.h> // mmap, munmap
#include <fcntl.h>   // open flags
```

## Quick Reference Commands:
- Compile: `gcc -o prog file.c -Wall -Wextra`
- Debug: `gcc -g file.c && gdb ./a.out`
- Memory check: `valgrind ./prog`