#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

// 1. Pointer to pointer (double pointer) operations
void demonstrate_double_pointers() {
    printf("=== Double Pointer Tricks ===\n");
    
    // Allocating 2D array dynamically
    int rows = 3, cols = 4;
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }
    
    // Access using different pointer arithmetic
    printf("Matrix access methods:\n");
    printf("matrix[1][2] = %d\n", matrix[1][2]);
    printf("*(*(matrix + 1) + 2) = %d\n", *(*(matrix + 1) + 2));
    printf("*((int *)matrix[1] + 2) = %d\n", *((int *)matrix[1] + 2));
    
    // Pointer swapping trick
    int *row0 = matrix[0];
    int *row2 = matrix[2];
    matrix[0] = row2;
    matrix[2] = row0;
    printf("\nAfter swapping rows 0 and 2:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }
    
    // Cleanup
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// 2. Function pointers in structures (virtual functions)
typedef struct {
    void (*print)(void *self);
    int (*compare)(void *self, void *other);
    void *data;
} VirtualObject;

void int_print(void *self) {
    VirtualObject *obj = (VirtualObject *)self;
    printf("Integer: %d\n", *(int *)obj->data);
}

int int_compare(void *self, void *other) {
    VirtualObject *obj1 = (VirtualObject *)self;
    VirtualObject *obj2 = (VirtualObject *)other;
    return *(int *)obj1->data - *(int *)obj2->data;
}

// 3. Pointer arithmetic with different types
void demonstrate_pointer_arithmetic() {
    printf("\n=== Tricky Pointer Arithmetic ===\n");
    
    // Array of mixed data in memory
    char buffer[100];
    
    // Store different types at specific offsets
    *(int *)(buffer + 0) = 42;
    *(double *)(buffer + 8) = 3.14159;
    strcpy(buffer + 16, "Hello");
    *(int *)(buffer + 24) = 100;
    
    // Read using pointer arithmetic
    printf("int at offset 0: %d\n", *(int *)buffer);
    printf("double at offset 8: %f\n", *(double *)(buffer + 8));
    printf("string at offset 16: %s\n", buffer + 16);
    printf("int at offset 24: %d\n", *(int *)(buffer + 24));
    
    // Pointer difference calculation
    int arr[] = {10, 20, 30, 40, 50};
    int *p1 = &arr[1];
    int *p2 = &arr[4];
    printf("\nPointer difference: p2 - p1 = %ld elements\n", p2 - p1);
    printf("Byte difference: %ld bytes\n", (char *)p2 - (char *)p1);
}

// 4. Void pointer tricks
void* generic_memswap(void *a, void *b, size_t size) {
    unsigned char *pa = (unsigned char *)a;
    unsigned char *pb = (unsigned char *)b;
    
    for (size_t i = 0; i < size; i++) {
        unsigned char temp = pa[i];
        pa[i] = pb[i];
        pb[i] = temp;
    }
    return a;
}

// 5. Pointer arrays vs array pointers
void demonstrate_pointer_arrays() {
    printf("\n=== Pointer Arrays vs Array Pointers ===\n");
    
    // Array of pointers
    int a = 1, b = 2, c = 3;
    int *arr_of_ptrs[3] = {&a, &b, &c};
    
    // Pointer to array
    int arr[3] = {10, 20, 30};
    int (*ptr_to_arr)[3] = &arr;
    
    printf("Array of pointers:\n");
    for (int i = 0; i < 3; i++) {
        printf("*arr_of_ptrs[%d] = %d\n", i, *arr_of_ptrs[i]);
    }
    
    printf("\nPointer to array:\n");
    for (int i = 0; i < 3; i++) {
        printf("(*ptr_to_arr)[%d] = %d\n", i, (*ptr_to_arr)[i]);
    }
    
    // Tricky: Using a static array
    static int static_arr[3] = {100, 200, 300};
    int (*result)[3] = &static_arr;
    printf("\nPointer to static array:\n");
    for (int i = 0; i < 3; i++) {
        printf("(*result)[%d] = %d\n", i, (*result)[i]);
    }
}

// 6. Self-referential structures and circular lists
typedef struct Node {
    int data;
    struct Node *next;
    struct Node **indirect;  // Pointer to pointer to Node
} Node;

void demonstrate_self_referential() {
    printf("\n=== Self-Referential Pointer Tricks ===\n");
    
    Node nodes[3] = {{1, NULL, NULL}, {2, NULL, NULL}, {3, NULL, NULL}};
    
    // Create circular list
    nodes[0].next = &nodes[1];
    nodes[1].next = &nodes[2];
    nodes[2].next = &nodes[0];
    
    // Set indirect pointers
    nodes[0].indirect = &nodes[1].next;
    nodes[1].indirect = &nodes[2].next;
    nodes[2].indirect = &nodes[0].next;
    
    printf("Circular traversal:\n");
    Node *current = &nodes[0];
    for (int i = 0; i < 6; i++) {
        printf("Node %d -> ", current->data);
        current = current->next;
    }
    printf("...\n");
    
    printf("\nIndirect access:\n");
    printf("nodes[0].indirect points to node with data: %d\n", 
           (*nodes[0].indirect)->data);
}

// 7. Alignment and padding tricks
void demonstrate_alignment() {
    printf("\n=== Alignment and Padding Tricks ===\n");
    
    struct Unaligned {
        char c;
        int i;
        char c2;
    };
    
    struct Aligned {
        char c;
        char c2;
        int i;
    };
    
    printf("Unaligned struct size: %zu\n", sizeof(struct Unaligned));
    printf("Aligned struct size: %zu\n", sizeof(struct Aligned));
    
    // Access members using offset calculation
    struct Unaligned u = {'A', 42, 'B'};
    char *base = (char *)&u;
    
    printf("\nManual offset access:\n");
    printf("c at offset 0: %c\n", *(char *)(base + 0));
    printf("i at offset %zu: %d\n", offsetof(struct Unaligned, i), 
           *(int *)(base + offsetof(struct Unaligned, i)));
    printf("c2 at offset %zu: %c\n", offsetof(struct Unaligned, c2), 
           *(char *)(base + offsetof(struct Unaligned, c2)));
}

// 8. Function pointer arrays (jump tables)
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_op(int a, int b) { return b ? a / b : 0; }

void demonstrate_function_pointers() {
    printf("\n=== Function Pointer Arrays ===\n");
    
    // Array of function pointers
    int (*operations[4])(int, int) = {add, sub, mul, div_op};
    char *op_names[] = {"add", "sub", "mul", "div"};
    
    int a = 10, b = 3;
    for (int i = 0; i < 4; i++) {
        printf("%d %s %d = %d\n", a, op_names[i], b, operations[i](a, b));
    }
    
    // Function pointer with variable arguments
    typedef int (*var_func)(int, ...);
    
    // 2D array of function pointers
    int (*op_matrix[2][2])(int, int) = {
        {add, sub},
        {mul, div_op}
    };
    
    printf("\nFunction pointer matrix:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("op_matrix[%d][%d](5, 3) = %d\n", i, j, op_matrix[i][j](5, 3));
        }
    }
}

// 9. Pointer bit manipulation
void demonstrate_pointer_bits() {
    printf("\n=== Pointer Bit Manipulation ===\n");
    
    // Using least significant bits for flags (assuming alignment)
    int value = 42;
    int *ptr = &value;
    
    // Store flags in lower bits (assumes 4-byte alignment)
    uintptr_t ptr_val = (uintptr_t)ptr;
    printf("Original pointer: %p\n", ptr);
    printf("Pointer value: 0x%lx\n", ptr_val);
    printf("Lower 2 bits: %ld\n", ptr_val & 3);
    
    // Tagged pointer example
    #define SET_TAG(p, tag) ((void *)((uintptr_t)(p) | (tag)))
    #define GET_TAG(p) ((uintptr_t)(p) & 3)
    #define CLEAR_TAG(p) ((void *)((uintptr_t)(p) & ~3UL))
    
    void *tagged = SET_TAG(ptr, 2);
    printf("\nTagged pointer: %p\n", tagged);
    printf("Tag value: %lu\n", GET_TAG(tagged));
    printf("Cleared pointer: %p\n", CLEAR_TAG(tagged));
}

// 10. Complex const pointer declarations
void demonstrate_const_pointers() {
    printf("\n=== Complex Const Pointers ===\n");
    
    int val = 10;
    const int c_val = 20;
    
    int *p1 = &val;                    // Pointer to int
    const int *p2 = &val;              // Pointer to const int
    int * const p3 = &val;             // Const pointer to int
    const int * const p4 = &val;       // Const pointer to const int
    
    // Array of const pointers to int
    int a = 1, b = 2, c = 3;
    int * const arr_cp[] = {&a, &b, &c};
    
    // Pointer to array of const int
    const int carr[3] = {10, 20, 30};
    const int (*p_to_carr)[3] = &carr;
    
    // Function pointer returning const pointer
    const int* (*func_ptr)() = NULL;
    
    printf("Reading through different pointer types:\n");
    printf("*p1 = %d\n", *p1);
    printf("*p2 = %d (can't modify through p2)\n", *p2);
    printf("*p3 = %d (can't change p3)\n", *p3);
    printf("*p4 = %d (can't change p4 or modify through it)\n", *p4);
}

int main() {
    demonstrate_double_pointers();
    demonstrate_pointer_arithmetic();
    demonstrate_pointer_arrays();
    demonstrate_self_referential();
    demonstrate_alignment();
    demonstrate_function_pointers();
    demonstrate_pointer_bits();
    demonstrate_const_pointers();
    
    // Void pointer swap example
    printf("\n=== Generic Swap Example ===\n");
    int x = 5, y = 10;
    double d1 = 3.14, d2 = 2.71;
    
    printf("Before swap: x=%d, y=%d\n", x, y);
    generic_memswap(&x, &y, sizeof(int));
    printf("After swap: x=%d, y=%d\n", x, y);
    
    printf("Before swap: d1=%f, d2=%f\n", d1, d2);
    generic_memswap(&d1, &d2, sizeof(double));
    printf("After swap: d1=%f, d2=%f\n", d1, d2);
    
    return 0;
}