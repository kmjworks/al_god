# Pointers Cheat Sheet - C Programming

## What are Pointers?
A pointer is a variable that stores the memory address of another variable. Instead of storing a value directly, it points to the location where the value is stored.

## Declaration and Initialization

```c
int x = 10;         // Regular variable
int *ptr;           // Pointer declaration
ptr = &x;           // ptr now holds the address of x
int *ptr2 = &x;     // Declaration and initialization combined
```

## Key Operators
- `&` - Address-of operator: Gets the address of a variable
- `*` - Dereference operator: Accesses the value at the address

## Basic Examples

### 1. Simple Pointer Usage
```c
#include <stdio.h>

int main() {
    int num = 42;
    int *ptr = &num;
    
    printf("Value of num: %d\n", num);           // 42
    printf("Address of num: %p\n", &num);        // Memory address
    printf("Value of ptr: %p\n", ptr);           // Same address
    printf("Value pointed by ptr: %d\n", *ptr);  // 42
    
    *ptr = 100;  // Change value through pointer
    printf("New value of num: %d\n", num);       // 100
    
    return 0;
}
```

### 2. Pointer Arithmetic
```c
int arr[5] = {10, 20, 30, 40, 50};
int *ptr = arr;  // Points to first element

printf("%d\n", *ptr);       // 10
printf("%d\n", *(ptr + 1)); // 20
printf("%d\n", *(ptr + 2)); // 30

ptr++;  // Move to next element
printf("%d\n", *ptr);       // 20
```

### 3. Pointers and Arrays
```c
int arr[5] = {1, 2, 3, 4, 5};
int *ptr = arr;  // Array name is a pointer to first element

// These are equivalent:
printf("%d\n", arr[2]);      // 3
printf("%d\n", *(arr + 2));  // 3
printf("%d\n", *(ptr + 2));  // 3
printf("%d\n", ptr[2]);      // 3
```

## Function Pointers
```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    // Function pointer declaration
    int (*operation)(int, int);
    
    operation = add;
    printf("5 + 3 = %d\n", operation(5, 3));  // 8
    
    operation = multiply;
    printf("5 * 3 = %d\n", operation(5, 3));  // 15
    
    return 0;
}
```

## Pointers to Pointers
```c
int x = 5;
int *ptr1 = &x;       // Single pointer
int **ptr2 = &ptr1;   // Pointer to pointer

printf("%d\n", x);        // 5
printf("%d\n", *ptr1);    // 5
printf("%d\n", **ptr2);   // 5

**ptr2 = 10;              // Modify x through double pointer
printf("%d\n", x);        // 10
```

## Common Patterns

### 1. Swap Function Using Pointers
```c
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 5, y = 10;
    printf("Before: x=%d, y=%d\n", x, y);  // 5, 10
    swap(&x, &y);
    printf("After: x=%d, y=%d\n", x, y);   // 10, 5
    return 0;
}
```

### 2. Dynamic Array Using Pointers
```c
#include <stdlib.h>

int main() {
    int n = 5;
    int *arr = (int*)malloc(n * sizeof(int));
    
    // Use as array
    for (int i = 0; i < n; i++) {
        arr[i] = i * 10;
    }
    
    // Print using pointer arithmetic
    for (int i = 0; i < n; i++) {
        printf("%d ", *(arr + i));  // 0 10 20 30 40
    }
    
    free(arr);  // Don't forget to free!
    return 0;
}
```

### 3. String Manipulation
```c
char str[] = "Hello";
char *ptr = str;

// Print string character by character
while (*ptr != '\0') {
    printf("%c", *ptr);
    ptr++;
}

// String length using pointers
char *p = str;
int len = 0;
while (*p++) {
    len++;
}
printf("\nLength: %d\n", len);  // 5
```

## NULL Pointers
```c
int *ptr = NULL;  // Initialize to NULL

// Always check before using
if (ptr != NULL) {
    *ptr = 10;  // Safe to use
} else {
    printf("Pointer is NULL\n");
}
```

## Void Pointers
```c
void *ptr;  // Can point to any data type
int x = 10;
float y = 3.14;

ptr = &x;
printf("%d\n", *(int*)ptr);    // Cast required

ptr = &y;
printf("%.2f\n", *(float*)ptr); // Cast to correct type
```

## Common Pitfalls and Solutions

### 1. Uninitialized Pointers
```c
// BAD
int *ptr;
*ptr = 10;  // Undefined behavior!

// GOOD
int *ptr = NULL;
// or
int x;
int *ptr = &x;
*ptr = 10;  // Safe
```

### 2. Dangling Pointers
```c
// BAD
int *get_value() {
    int x = 10;
    return &x;  // x is destroyed after function returns!
}

// GOOD
int *get_value() {
    int *x = (int*)malloc(sizeof(int));
    *x = 10;
    return x;  // Caller must free
}
```

### 3. Memory Leaks
```c
// BAD
int *ptr = (int*)malloc(sizeof(int));
ptr = NULL;  // Memory leaked!

// GOOD
int *ptr = (int*)malloc(sizeof(int));
free(ptr);
ptr = NULL;  // Good practice
```

## Quick Reference
| Operation | Syntax | Example |
|-----------|--------|---------|
| Declare pointer | `type *ptr;` | `int *p;` |
| Get address | `&variable` | `p = &x;` |
| Dereference | `*pointer` | `y = *p;` |
| NULL check | `if (ptr != NULL)` | Safety check |
| Pointer arithmetic | `ptr + n` | Move n elements |
| Array access | `ptr[i]` or `*(ptr + i)` | Access element |

## Exam Tips
1. Always initialize pointers (to NULL or valid address)
2. Check for NULL before dereferencing
3. Free dynamically allocated memory
4. Remember: array name is a pointer to first element
5. Pointer arithmetic moves by sizeof(type)
6. Function parameters can be pointers for "pass by reference"