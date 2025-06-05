#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>

// Memory block header
typedef struct Block {
    size_t size;          // Size of the block (excluding header)
    struct Block *next;   // Next free block
    struct Block *prev;   // Previous free block
    int free;            // 1 if free, 0 if allocated
    int magic;           // Magic number for corruption detection
} Block;

#define BLOCK_SIZE sizeof(Block)
#define MAGIC_FREE 0xDEADBEEF
#define MAGIC_ALLOC 0xBEEFDEAD
#define ALIGN_SIZE 8
#define MIN_BLOCK_SIZE 16

// Align size to 8-byte boundary
size_t align_size(size_t size) {
    return (size + ALIGN_SIZE - 1) & ~(ALIGN_SIZE - 1);
}

// Custom allocator structure
typedef struct {
    void *heap_start;
    void *heap_end;
    size_t heap_size;
    Block *free_list;
    size_t allocated_bytes;
    size_t free_bytes;
    int allocation_count;
    int free_count;
} Allocator;

// Global allocator instance
Allocator g_allocator = {0};

// Initialize allocator with fixed heap size
void allocator_init(size_t heap_size) {
    g_allocator.heap_size = heap_size;
    g_allocator.heap_start = malloc(heap_size);
    if (!g_allocator.heap_start) {
        fprintf(stderr, "Failed to allocate heap\n");
        exit(1);
    }
    
    g_allocator.heap_end = (char *)g_allocator.heap_start + heap_size;
    
    // Initialize the first free block
    Block *initial = (Block *)g_allocator.heap_start;
    initial->size = heap_size - BLOCK_SIZE;
    initial->next = NULL;
    initial->prev = NULL;
    initial->free = 1;
    initial->magic = MAGIC_FREE;
    
    g_allocator.free_list = initial;
    g_allocator.free_bytes = initial->size;
    g_allocator.allocated_bytes = 0;
    g_allocator.allocation_count = 0;
    g_allocator.free_count = 0;
}

// Find a suitable free block using first-fit strategy
Block* find_free_block(size_t size) {
    Block *current = g_allocator.free_list;
    
    while (current) {
        if (current->free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Split a block if it's too large
void split_block(Block *block, size_t size) {
    // Only split if remaining size is large enough
    if (block->size >= size + BLOCK_SIZE + MIN_BLOCK_SIZE) {
        Block *new_block = (Block *)((char *)block + BLOCK_SIZE + size);
        new_block->size = block->size - size - BLOCK_SIZE;
        new_block->free = 1;
        new_block->magic = MAGIC_FREE;
        new_block->next = block->next;
        new_block->prev = block;
        
        if (block->next) {
            block->next->prev = new_block;
        }
        
        block->next = new_block;
        block->size = size;
    }
}

// Merge adjacent free blocks
void coalesce_blocks() {
    Block *current = g_allocator.free_list;
    
    while (current && current->next) {
        if (current->free && current->next->free) {
            // Check if blocks are adjacent
            Block *next_expected = (Block *)((char *)current + BLOCK_SIZE + current->size);
            if (next_expected == current->next) {
                // Merge blocks
                current->size += BLOCK_SIZE + current->next->size;
                current->next = current->next->next;
                if (current->next) {
                    current->next->prev = current;
                }
            } else {
                current = current->next;
            }
        } else {
            current = current->next;
        }
    }
}

// Custom malloc implementation
void* my_malloc(size_t size) {
    if (size == 0) return NULL;
    
    size = align_size(size);
    Block *block = find_free_block(size);
    
    if (!block) {
        fprintf(stderr, "Out of memory! Requested: %zu bytes\n", size);
        return NULL;
    }
    
    // Split block if necessary
    split_block(block, size);
    
    // Mark block as allocated
    block->free = 0;
    block->magic = MAGIC_ALLOC;
    
    // Update statistics
    g_allocator.allocated_bytes += block->size;
    g_allocator.free_bytes -= block->size;
    g_allocator.allocation_count++;
    
    // Return pointer to usable memory (after header)
    return (char *)block + BLOCK_SIZE;
}

// Custom free implementation
void my_free(void *ptr) {
    if (!ptr) return;
    
    // Get block header
    Block *block = (Block *)((char *)ptr - BLOCK_SIZE);
    
    // Validate magic number
    if (block->magic != MAGIC_ALLOC) {
        fprintf(stderr, "Corruption detected or double free!\n");
        return;
    }
    
    // Mark as free
    block->free = 1;
    block->magic = MAGIC_FREE;
    
    // Update statistics
    g_allocator.allocated_bytes -= block->size;
    g_allocator.free_bytes += block->size;
    g_allocator.free_count++;
    
    // Coalesce adjacent free blocks
    coalesce_blocks();
}

// Custom realloc implementation
void* my_realloc(void *ptr, size_t new_size) {
    if (!ptr) return my_malloc(new_size);
    if (new_size == 0) {
        my_free(ptr);
        return NULL;
    }
    
    Block *block = (Block *)((char *)ptr - BLOCK_SIZE);
    size_t old_size = block->size;
    
    // If new size fits in current block, return same pointer
    if (new_size <= old_size) {
        return ptr;
    }
    
    // Allocate new block and copy data
    void *new_ptr = my_malloc(new_size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, old_size);
        my_free(ptr);
    }
    
    return new_ptr;
}

// Custom calloc implementation
void* my_calloc(size_t num, size_t size) {
    size_t total = num * size;
    void *ptr = my_malloc(total);
    
    if (ptr) {
        memset(ptr, 0, total);
    }
    
    return ptr;
}

// Print allocator statistics
void print_stats() {
    printf("\n=== Allocator Statistics ===\n");
    printf("Heap size: %zu bytes\n", g_allocator.heap_size);
    printf("Allocated: %zu bytes\n", g_allocator.allocated_bytes);
    printf("Free: %zu bytes\n", g_allocator.free_bytes);
    printf("Allocations: %d\n", g_allocator.allocation_count);
    printf("Frees: %d\n", g_allocator.free_count);
    printf("Fragmentation: %.2f%%\n", 
           (g_allocator.free_bytes > 0) ? 
           (100.0 * (1.0 - (double)largest_free_block() / g_allocator.free_bytes)) : 0);
}

// Get size of largest free block
size_t largest_free_block() {
    Block *current = g_allocator.free_list;
    size_t max_size = 0;
    
    while (current) {
        if (current->free && current->size > max_size) {
            max_size = current->size;
        }
        current = current->next;
    }
    
    return max_size;
}

// Visualize heap layout
void visualize_heap() {
    printf("\n=== Heap Layout ===\n");
    Block *current = (Block *)g_allocator.heap_start;
    int block_num = 0;
    
    while ((void *)current < g_allocator.heap_end) {
        printf("Block %d: ", block_num++);
        printf("[%s] ", current->free ? "FREE" : "USED");
        printf("Size: %zu bytes, ", current->size);
        printf("Address: %p\n", current);
        
        // Move to next block
        current = (Block *)((char *)current + BLOCK_SIZE + current->size);
        
        // Safety check
        if (block_num > 100) break;
    }
}

// Memory pool allocator for fixed-size objects
typedef struct PoolBlock {
    struct PoolBlock *next;
} PoolBlock;

typedef struct {
    void *memory;
    PoolBlock *free_list;
    size_t block_size;
    size_t num_blocks;
    int allocated_count;
} MemoryPool;

// Initialize memory pool
MemoryPool* pool_create(size_t block_size, size_t num_blocks) {
    MemoryPool *pool = malloc(sizeof(MemoryPool));
    if (!pool) return NULL;
    
    pool->block_size = align_size(block_size);
    pool->num_blocks = num_blocks;
    pool->allocated_count = 0;
    
    // Allocate memory for all blocks
    pool->memory = malloc(pool->block_size * num_blocks);
    if (!pool->memory) {
        free(pool);
        return NULL;
    }
    
    // Initialize free list
    pool->free_list = (PoolBlock *)pool->memory;
    PoolBlock *current = pool->free_list;
    
    for (size_t i = 1; i < num_blocks; i++) {
        current->next = (PoolBlock *)((char *)pool->memory + i * pool->block_size);
        current = current->next;
    }
    current->next = NULL;
    
    return pool;
}

// Allocate from pool
void* pool_alloc(MemoryPool *pool) {
    if (!pool->free_list) return NULL;
    
    PoolBlock *block = pool->free_list;
    pool->free_list = block->next;
    pool->allocated_count++;
    
    return block;
}

// Free to pool
void pool_free(MemoryPool *pool, void *ptr) {
    if (!ptr) return;
    
    PoolBlock *block = (PoolBlock *)ptr;
    block->next = pool->free_list;
    pool->free_list = block;
    pool->allocated_count--;
}

// Destroy pool
void pool_destroy(MemoryPool *pool) {
    free(pool->memory);
    free(pool);
}

// Test the allocators
int main() {
    printf("=== Custom Memory Allocator Demo ===\n");
    
    // Initialize allocator with 10KB heap
    allocator_init(10240);
    
    // Test 1: Basic allocation and free
    printf("\n1. Basic allocation test:\n");
    void *p1 = my_malloc(100);
    void *p2 = my_malloc(200);
    void *p3 = my_malloc(150);
    
    printf("Allocated 3 blocks\n");
    print_stats();
    
    my_free(p2);
    printf("\nFreed middle block\n");
    print_stats();
    
    // Test 2: Reallocation
    printf("\n2. Reallocation test:\n");
    void *p4 = my_malloc(50);
    strcpy(p4, "Hello, World!");
    printf("Original string: %s\n", (char *)p4);
    
    p4 = my_realloc(p4, 100);
    printf("After realloc: %s\n", (char *)p4);
    
    // Test 3: Calloc
    printf("\n3. Calloc test:\n");
    int *arr = my_calloc(10, sizeof(int));
    printf("Calloc'd array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // Test 4: Fragmentation
    printf("\n4. Fragmentation test:\n");
    void *blocks[20];
    for (int i = 0; i < 20; i++) {
        blocks[i] = my_malloc(100);
    }
    
    // Free every other block
    for (int i = 0; i < 20; i += 2) {
        my_free(blocks[i]);
    }
    
    print_stats();
    visualize_heap();
    
    // Test 5: Memory pool
    printf("\n5. Memory pool test:\n");
    MemoryPool *pool = pool_create(sizeof(int), 100);
    
    int *pool_ints[50];
    for (int i = 0; i < 50; i++) {
        pool_ints[i] = pool_alloc(pool);
        *pool_ints[i] = i * i;
    }
    
    printf("Pool allocated 50 integers\n");
    printf("Pool allocation count: %d\n", pool->allocated_count);
    
    // Free half
    for (int i = 0; i < 25; i++) {
        pool_free(pool, pool_ints[i]);
    }
    
    printf("After freeing 25: %d allocated\n", pool->allocated_count);
    
    // Cleanup
    pool_destroy(pool);
    
    // Final statistics
    printf("\n=== Final Statistics ===\n");
    print_stats();
    
    // Free remaining allocations
    my_free(p1);
    my_free(p3);
    my_free(p4);
    my_free(arr);
    for (int i = 1; i < 20; i += 2) {
        my_free(blocks[i]);
    }
    
    printf("\nAfter cleanup:\n");
    print_stats();
    
    // Cleanup allocator
    free(g_allocator.heap_start);
    
    return 0;
}