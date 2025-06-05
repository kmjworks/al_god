#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void basic_pointer_demo() {
    printf("\n=== Basic Pointer Demo ===\n");
    int x = 42;
    int *ptr = &x;
    
    printf("x = %d\n", x);
    printf("Address of x = %p\n", (void*)&x);
    printf("ptr = %p\n", (void*)ptr);
    printf("*ptr = %d\n", *ptr);
    
    *ptr = 100;
    printf("After *ptr = 100, x = %d\n", x);
}

void pointer_arithmetic_demo() {
    printf("\n=== Pointer Arithmetic Demo ===\n");
    int arr[] = {10, 20, 30, 40, 50};
    int *ptr = arr;
    
    for (int i = 0; i < 5; i++) {
        printf("ptr + %d points to %d\n", i, *(ptr + i));
    }
    
    printf("\nUsing increment:\n");
    ptr = arr;
    for (int i = 0; i < 5; i++) {
        printf("*ptr++ = %d\n", *ptr++);
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void modify_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= 2;
    }
}

void function_with_pointers() {
    printf("\n=== Function with Pointers Demo ===\n");
    int x = 5, y = 10;
    printf("Before swap: x = %d, y = %d\n", x, y);
    swap(&x, &y);
    printf("After swap: x = %d, y = %d\n", x, y);
    
    int numbers[] = {1, 2, 3, 4, 5};
    printf("\nBefore modify: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    
    modify_array(numbers, 5);
    printf("\nAfter modify: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
}

void double_pointer_demo() {
    printf("\n=== Double Pointer Demo ===\n");
    int x = 5;
    int *ptr1 = &x;
    int **ptr2 = &ptr1;
    
    printf("x = %d\n", x);
    printf("*ptr1 = %d\n", *ptr1);
    printf("**ptr2 = %d\n", **ptr2);
    
    **ptr2 = 20;
    printf("After **ptr2 = 20, x = %d\n", x);
}

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }

void function_pointer_demo() {
    printf("\n=== Function Pointer Demo ===\n");
    int (*operation)(int, int);
    
    operation = add;
    printf("5 + 3 = %d\n", operation(5, 3));
    
    operation = subtract;
    printf("5 - 3 = %d\n", operation(5, 3));
    
    operation = multiply;
    printf("5 * 3 = %d\n", operation(5, 3));
    
    int (*ops[])(int, int) = {add, subtract, multiply};
    char *op_names[] = {"add", "subtract", "multiply"};
    
    printf("\nUsing function pointer array:\n");
    for (int i = 0; i < 3; i++) {
        printf("%s(10, 5) = %d\n", op_names[i], ops[i](10, 5));
    }
}

void string_pointer_demo() {
    printf("\n=== String Pointer Demo ===\n");
    char str[] = "Hello, World!";
    char *ptr = str;
    
    printf("Using array index: ");
    for (int i = 0; str[i] != '\0'; i++) {
        printf("%c", str[i]);
    }
    
    printf("\nUsing pointer: ");
    while (*ptr) {
        printf("%c", *ptr++);
    }
    
    ptr = str;
    printf("\nString length: ");
    int len = 0;
    while (*ptr++) len++;
    printf("%d\n", len);
}

void dynamic_memory_demo() {
    printf("\n=== Dynamic Memory with Pointers ===\n");
    int n = 5;
    int *arr = (int*)malloc(n * sizeof(int));
    
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    for (int i = 0; i < n; i++) {
        arr[i] = (i + 1) * 10;
    }
    
    printf("Dynamic array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");
    
    int **matrix = (int**)malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++) {
        matrix[i] = (int*)malloc(3 * sizeof(int));
        for (int j = 0; j < 3; j++) {
            matrix[i][j] = i * 3 + j;
        }
    }
    
    printf("\nDynamic 2D array:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < 3; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(arr);
}

void void_pointer_demo() {
    printf("\n=== Void Pointer Demo ===\n");
    void *ptr;
    int x = 10;
    float y = 3.14;
    char c = 'A';
    
    ptr = &x;
    printf("Integer: %d\n", *(int*)ptr);
    
    ptr = &y;
    printf("Float: %.2f\n", *(float*)ptr);
    
    ptr = &c;
    printf("Char: %c\n", *(char*)ptr);
}

struct Point {
    int x;
    int y;
};

void struct_pointer_demo() {
    printf("\n=== Struct Pointer Demo ===\n");
    struct Point p1 = {10, 20};
    struct Point *ptr = &p1;
    
    printf("Using dot operator: p1.x = %d, p1.y = %d\n", p1.x, p1.y);
    printf("Using arrow operator: ptr->x = %d, ptr->y = %d\n", ptr->x, ptr->y);
    printf("Using dereference: (*ptr).x = %d, (*ptr).y = %d\n", (*ptr).x, (*ptr).y);
    
    ptr->x = 30;
    ptr->y = 40;
    printf("After modification: p1.x = %d, p1.y = %d\n", p1.x, p1.y);
}

int main() {
    printf("POINTER COMPREHENSIVE EXAMPLES\n");
    printf("==============================\n");
    
    basic_pointer_demo();
    pointer_arithmetic_demo();
    function_with_pointers();
    double_pointer_demo();
    function_pointer_demo();
    string_pointer_demo();
    dynamic_memory_demo();
    void_pointer_demo();
    struct_pointer_demo();
    
    return 0;
}