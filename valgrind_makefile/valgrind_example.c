#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

// Example 1: Memory leak
void memory_leak_example() {
    LOG_INFO("Memory leak example");
    
    // Allocate memory but don't free it
    int *leaked = malloc(100 * sizeof(int));
    for (int i = 0; i < 100; i++) {
        leaked[i] = i;
    }
    // Missing: free(leaked);
}

// Example 2: Invalid write
void invalid_write_example() {
    LOG_INFO("Invalid write example");
    
    int *arr = malloc(5 * sizeof(int));
    
    // Write beyond allocated memory
    for (int i = 0; i <= 5; i++) {  // Bug: should be i < 5
        arr[i] = i;
    }
    
    free(arr);
}

// Example 3: Use after free
void use_after_free_example() {
    LOG_INFO("Use after free example");
    
    int *ptr = malloc(sizeof(int));
    *ptr = 42;
    
    free(ptr);
    
    // Using freed memory
    printf("Value: %d\n", *ptr);  // Bug: accessing freed memory
}

// Example 4: Double free
void double_free_example() {
    LOG_INFO("Double free example");
    
    int *ptr = malloc(sizeof(int));
    *ptr = 42;
    
    free(ptr);
    // free(ptr);  // Bug: freeing already freed memory
    // Commented out to prevent crash
}

// Example 5: Uninitialized value
void uninitialized_value_example() {
    LOG_INFO("Uninitialized value example");
    
    int x;  // Not initialized
    int y = 5;
    
    if (x > 0) {  // Bug: x is uninitialized
        y = x + 10;
    }
    
    printf("y = %d\n", y);
}

// Example 6: Correct memory usage
void correct_memory_usage() {
    LOG_INFO("Correct memory usage example");
    
    // Proper allocation
    int *arr = malloc(10 * sizeof(int));
    if (!arr) {
        LOG_ERROR("Memory allocation failed");
        return;
    }
    
    // Initialize memory
    for (int i = 0; i < 10; i++) {
        arr[i] = i * i;
    }
    
    // Use memory
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += arr[i];
    }
    LOG_DEBUG("Sum of squares: %d", sum);
    
    // Free memory
    free(arr);
    arr = NULL;  // Good practice
}

// Example 7: String operations
void string_operations_example() {
    LOG_INFO("String operations example");
    
    // Correct string allocation
    char *str = malloc(50 * sizeof(char));
    if (!str) {
        LOG_ERROR("Memory allocation failed");
        return;
    }
    
    // Safe string copy
    strncpy(str, "Hello, Valgrind!", 49);
    str[49] = '\0';  // Ensure null termination
    
    LOG_DEBUG("String: %s", str);
    
    // String concatenation with bounds checking
    if (strlen(str) + strlen(" Testing") < 50) {
        strcat(str, " Testing");
    }
    
    LOG_DEBUG("Concatenated: %s", str);
    
    free(str);
}

// Example 8: Dynamic 2D array
void dynamic_2d_array_example() {
    LOG_INFO("Dynamic 2D array example");
    
    int rows = 3, cols = 4;
    
    // Allocate array of pointers
    int **matrix = malloc(rows * sizeof(int*));
    if (!matrix) {
        LOG_ERROR("Memory allocation failed");
        return;
    }
    
    // Allocate each row
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (!matrix[i]) {
            // Clean up already allocated rows
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            LOG_ERROR("Memory allocation failed for row %d", i);
            return;
        }
    }
    
    // Initialize and use matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }
    
    // Print matrix
    LOG_DEBUG("Matrix contents:");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
    
    // Free memory in reverse order
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Example 9: Memory reallocation
void reallocation_example() {
    LOG_INFO("Memory reallocation example");
    
    int *arr = malloc(5 * sizeof(int));
    if (!arr) {
        LOG_ERROR("Initial allocation failed");
        return;
    }
    
    // Fill initial array
    for (int i = 0; i < 5; i++) {
        arr[i] = i;
    }
    
    // Reallocate to larger size
    int *temp = realloc(arr, 10 * sizeof(int));
    if (!temp) {
        LOG_ERROR("Reallocation failed");
        free(arr);
        return;
    }
    arr = temp;
    
    // Fill new elements
    for (int i = 5; i < 10; i++) {
        arr[i] = i;
    }
    
    LOG_DEBUG("Reallocated array:");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    free(arr);
}

int main() {
    // Initialize logger
    LogConfig config = {
        .min_level = LOG_DEBUG,
        .use_colors = true,
        .log_to_file = true,
        .log_to_console = true,
        .include_timestamp = true,
        .include_file_info = true,
        .log_file_path = "valgrind_test.log",
        .max_file_size = 1024 * 1024,  // 1MB
        .max_backup_files = 3
    };
    
    g_logger = logger_create(&config);
    if (!g_logger) {
        fprintf(stderr, "Failed to create logger\n");
        return 1;
    }
    
    LOG_INFO("Starting Valgrind example program");
    
    // Run examples (comment out the buggy ones to see clean valgrind output)
    memory_leak_example();        // Will show memory leak
    invalid_write_example();      // Will show invalid write
    // use_after_free_example();  // Commented to prevent undefined behavior
    // double_free_example();     // Commented to prevent crash
    uninitialized_value_example(); // Will show uninitialized value use
    
    // Correct examples
    correct_memory_usage();
    string_operations_example();
    dynamic_2d_array_example();
    reallocation_example();
    
    LOG_INFO("Program completed");
    
    // Clean up logger
    logger_destroy(g_logger);
    
    return 0;
}

/*
 * To compile and run with Valgrind:
 * 
 * gcc -g -O0 -o valgrind_example valgrind_example.c logger.c -pthread
 * valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./valgrind_example
 * 
 * Expected Valgrind output will show:
 * 1. Memory leak from memory_leak_example()
 * 2. Invalid write in invalid_write_example()
 * 3. Conditional jump depends on uninitialized value in uninitialized_value_example()
 * 4. Clean memory usage for the correct examples
 */