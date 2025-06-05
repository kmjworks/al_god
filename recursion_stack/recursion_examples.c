#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Stack visualization helper
void print_stack_frame(const char* func_name, int depth, const char* info) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    printf("→ %s(%s)\n", func_name, info);
}

// Basic recursion examples
int factorial_traced(int n, int depth) {
    char info[50];
    sprintf(info, "n=%d", n);
    print_stack_frame("factorial", depth, info);
    
    if (n <= 1) {
        printf("%*sReturning: 1\n", depth * 2, "");
        return 1;
    }
    
    int result = n * factorial_traced(n - 1, depth + 1);
    printf("%*sReturning: %d\n", depth * 2, "", result);
    return result;
}

void factorial_demo() {
    printf("\n=== Factorial with Stack Trace ===\n");
    int n = 5;
    printf("Computing factorial(%d):\n", n);
    int result = factorial_traced(n, 0);
    printf("Result: %d\n", result);
}

// Fibonacci with memoization comparison
int fib_calls = 0;

int fibonacci_naive(int n) {
    fib_calls++;
    if (n <= 1) return n;
    return fibonacci_naive(n - 1) + fibonacci_naive(n - 2);
}

int fibonacci_memo_helper(int n, int* memo) {
    fib_calls++;
    if (n <= 1) return n;
    
    if (memo[n] != -1) {
        return memo[n];
    }
    
    memo[n] = fibonacci_memo_helper(n - 1, memo) + 
              fibonacci_memo_helper(n - 2, memo);
    return memo[n];
}

int fibonacci_memo(int n) {
    int* memo = malloc((n + 1) * sizeof(int));
    for (int i = 0; i <= n; i++) {
        memo[i] = -1;
    }
    
    int result = fibonacci_memo_helper(n, memo);
    free(memo);
    return result;
}

void fibonacci_demo() {
    printf("\n=== Fibonacci Comparison ===\n");
    int n = 20;
    
    // Naive approach
    fib_calls = 0;
    clock_t start = clock();
    int result1 = fibonacci_naive(n);
    clock_t end = clock();
    double time1 = ((double)(end - start)) / CLOCKS_PER_SEC;
    int calls1 = fib_calls;
    
    // Memoized approach
    fib_calls = 0;
    start = clock();
    int result2 = fibonacci_memo(n);
    end = clock();
    double time2 = ((double)(end - start)) / CLOCKS_PER_SEC;
    int calls2 = fib_calls;
    
    printf("Fibonacci(%d):\n", n);
    printf("Naive:     Result=%d, Calls=%d, Time=%.6f seconds\n", 
           result1, calls1, time1);
    printf("Memoized:  Result=%d, Calls=%d, Time=%.6f seconds\n", 
           result2, calls2, time2);
    printf("Speedup: %.2fx fewer calls\n", (double)calls1 / calls2);
}

// Array operations
void print_array(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]");
}

int find_max_recursive(int arr[], int n) {
    if (n == 1) {
        return arr[0];
    }
    
    int max_of_rest = find_max_recursive(arr, n - 1);
    return (arr[n - 1] > max_of_rest) ? arr[n - 1] : max_of_rest;
}

bool is_sorted_recursive(int arr[], int n) {
    if (n <= 1) {
        return true;
    }
    
    if (arr[n - 2] > arr[n - 1]) {
        return false;
    }
    
    return is_sorted_recursive(arr, n - 1);
}

void array_recursion_demo() {
    printf("\n=== Array Recursion Demo ===\n");
    
    int arr1[] = {3, 7, 2, 9, 1, 5};
    int size1 = 6;
    printf("Array: ");
    print_array(arr1, size1);
    printf("\n");
    
    int max = find_max_recursive(arr1, size1);
    printf("Maximum element: %d\n", max);
    
    int arr2[] = {1, 3, 5, 7, 9};
    int arr3[] = {1, 3, 2, 7, 9};
    
    printf("\nChecking if arrays are sorted:\n");
    print_array(arr2, 5);
    printf(" -> %s\n", is_sorted_recursive(arr2, 5) ? "Sorted" : "Not sorted");
    
    print_array(arr3, 5);
    printf(" -> %s\n", is_sorted_recursive(arr3, 5) ? "Sorted" : "Not sorted");
}

// String recursion
void reverse_string_recursive(char* str, int start, int end) {
    if (start >= end) {
        return;
    }
    
    // Swap
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    
    reverse_string_recursive(str, start + 1, end - 1);
}

int count_char_recursive(const char* str, char ch) {
    if (*str == '\0') {
        return 0;
    }
    
    int count = (*str == ch) ? 1 : 0;
    return count + count_char_recursive(str + 1, ch);
}

void remove_spaces_recursive(char* src, char* dest, int i, int j) {
    if (src[i] == '\0') {
        dest[j] = '\0';
        return;
    }
    
    if (src[i] != ' ') {
        dest[j] = src[i];
        remove_spaces_recursive(src, dest, i + 1, j + 1);
    } else {
        remove_spaces_recursive(src, dest, i + 1, j);
    }
}

void string_recursion_demo() {
    printf("\n=== String Recursion Demo ===\n");
    
    // Reverse string
    char str1[] = "Hello World";
    printf("Original: '%s'\n", str1);
    reverse_string_recursive(str1, 0, strlen(str1) - 1);
    printf("Reversed: '%s'\n", str1);
    
    // Count characters
    const char* str2 = "mississippi";
    printf("\nCounting characters in '%s':\n", str2);
    printf("'s' appears %d times\n", count_char_recursive(str2, 's'));
    printf("'i' appears %d times\n", count_char_recursive(str2, 'i'));
    
    // Remove spaces
    char str3[] = "H e l l o   W o r l d";
    char result[50];
    printf("\nRemoving spaces from: '%s'\n", str3);
    remove_spaces_recursive(str3, result, 0, 0);
    printf("Result: '%s'\n", result);
}

// Binary search variations
int binary_search_first(int arr[], int left, int right, int target) {
    if (left > right) {
        return -1;
    }
    
    int mid = left + (right - left) / 2;
    
    if (arr[mid] == target) {
        // Check if it's the first occurrence
        if (mid == 0 || arr[mid - 1] != target) {
            return mid;
        }
        return binary_search_first(arr, left, mid - 1, target);
    }
    
    if (arr[mid] > target) {
        return binary_search_first(arr, left, mid - 1, target);
    }
    
    return binary_search_first(arr, mid + 1, right, target);
}

int binary_search_last(int arr[], int left, int right, int target, int n) {
    if (left > right) {
        return -1;
    }
    
    int mid = left + (right - left) / 2;
    
    if (arr[mid] == target) {
        // Check if it's the last occurrence
        if (mid == n - 1 || arr[mid + 1] != target) {
            return mid;
        }
        return binary_search_last(arr, mid + 1, right, target, n);
    }
    
    if (arr[mid] > target) {
        return binary_search_last(arr, left, mid - 1, target, n);
    }
    
    return binary_search_last(arr, mid + 1, right, target, n);
}

void binary_search_demo() {
    printf("\n=== Binary Search Variations ===\n");
    
    int arr[] = {1, 2, 2, 2, 3, 4, 4, 5, 6};
    int n = 9;
    
    printf("Array: ");
    print_array(arr, n);
    printf("\n");
    
    int target = 2;
    int first = binary_search_first(arr, 0, n - 1, target);
    int last = binary_search_last(arr, 0, n - 1, target, n);
    
    printf("Target %d: First occurrence at index %d, Last at index %d\n", 
           target, first, last);
    
    target = 4;
    first = binary_search_first(arr, 0, n - 1, target);
    last = binary_search_last(arr, 0, n - 1, target, n);
    
    printf("Target %d: First occurrence at index %d, Last at index %d\n", 
           target, first, last);
}

// Tower of Hanoi with visualization
void print_hanoi_state(int A[], int B[], int C[], int nA, int nB, int nC) {
    printf("A: ");
    for (int i = 0; i < nA; i++) printf("%d ", A[i]);
    printf("\nB: ");
    for (int i = 0; i < nB; i++) printf("%d ", B[i]);
    printf("\nC: ");
    for (int i = 0; i < nC; i++) printf("%d ", C[i]);
    printf("\n---\n");
}

int move_count = 0;

void tower_of_hanoi(int n, char from, char to, char aux) {
    if (n == 1) {
        move_count++;
        printf("Move %d: disk 1 from %c to %c\n", move_count, from, to);
        return;
    }
    
    tower_of_hanoi(n - 1, from, aux, to);
    move_count++;
    printf("Move %d: disk %d from %c to %c\n", move_count, n, from, to);
    tower_of_hanoi(n - 1, aux, to, from);
}

void hanoi_demo() {
    printf("\n=== Tower of Hanoi Demo ===\n");
    
    int n = 3;
    printf("Solving Tower of Hanoi with %d disks:\n", n);
    move_count = 0;
    
    tower_of_hanoi(n, 'A', 'C', 'B');
    
    printf("\nTotal moves: %d (Expected: %d)\n", move_count, (1 << n) - 1);
}

// Permutations generator
void swap_chars(char* a, char* b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

int perm_count = 0;

void generate_permutations(char* str, int left, int right) {
    if (left == right) {
        perm_count++;
        printf("%d: %s\n", perm_count, str);
        return;
    }
    
    for (int i = left; i <= right; i++) {
        swap_chars(&str[left], &str[i]);
        generate_permutations(str, left + 1, right);
        swap_chars(&str[left], &str[i]);  // Backtrack
    }
}

void permutation_demo() {
    printf("\n=== Permutation Generation ===\n");
    
    char str[] = "ABC";
    printf("Generating all permutations of '%s':\n", str);
    perm_count = 0;
    
    generate_permutations(str, 0, strlen(str) - 1);
    
    printf("\nTotal permutations: %d\n", perm_count);
}

// Subset generation
void print_subset(int arr[], int subset[], int subset_size) {
    printf("{ ");
    for (int i = 0; i < subset_size; i++) {
        printf("%d ", subset[i]);
    }
    printf("}\n");
}

void generate_subsets(int arr[], int n, int subset[], int subset_size, int index) {
    // Print current subset
    print_subset(arr, subset, subset_size);
    
    // Generate subsets by including remaining elements
    for (int i = index; i < n; i++) {
        subset[subset_size] = arr[i];
        generate_subsets(arr, n, subset, subset_size + 1, i + 1);
    }
}

void subset_demo() {
    printf("\n=== Subset Generation ===\n");
    
    int arr[] = {1, 2, 3};
    int n = 3;
    int subset[10];
    
    printf("All subsets of ");
    print_array(arr, n);
    printf(":\n");
    
    generate_subsets(arr, n, subset, 0, 0);
}

// Merge sort with trace
void merge_traced(int arr[], int left, int mid, int right, int depth) {
    char info[100];
    sprintf(info, "left=%d, mid=%d, right=%d", left, mid, right);
    print_stack_frame("merge", depth, info);
    
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int* L = malloc(n1 * sizeof(int));
    int* R = malloc(n2 * sizeof(int));
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    
    free(L);
    free(R);
}

void merge_sort_traced(int arr[], int left, int right, int depth) {
    char info[100];
    sprintf(info, "left=%d, right=%d", left, right);
    print_stack_frame("merge_sort", depth, info);
    
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort_traced(arr, left, mid, depth + 1);
        merge_sort_traced(arr, mid + 1, right, depth + 1);
        merge_traced(arr, left, mid, right, depth + 1);
    }
}

void merge_sort_demo() {
    printf("\n=== Merge Sort with Trace ===\n");
    
    int arr[] = {38, 27, 43, 3, 9, 82, 10};
    int n = 7;
    
    printf("Original array: ");
    print_array(arr, n);
    printf("\n\nSorting process:\n");
    
    merge_sort_traced(arr, 0, n - 1, 0);
    
    printf("\nSorted array: ");
    print_array(arr, n);
    printf("\n");
}

// Tail recursion examples
int gcd_regular(int a, int b, int depth) {
    printf("%*sGCD(%d, %d)\n", depth * 2, "", a, b);
    
    if (b == 0) {
        return a;
    }
    
    return gcd_regular(b, a % b, depth + 1);
}

int sum_tail_recursive(int n, int accumulator) {
    if (n == 0) {
        return accumulator;
    }
    return sum_tail_recursive(n - 1, accumulator + n);
}

void tail_recursion_demo() {
    printf("\n=== Tail Recursion Demo ===\n");
    
    printf("GCD computation (tail recursive):\n");
    int result = gcd_regular(48, 18, 0);
    printf("Result: %d\n", result);
    
    printf("\nSum 1 to 100 (tail recursive): %d\n", sum_tail_recursive(100, 0));
}

// Stack depth testing
int max_depth_reached = 0;

void test_stack_depth(int depth) {
    max_depth_reached = depth;
    
    // Create some local variables to use stack space
    char buffer[100];
    sprintf(buffer, "Depth %d", depth);
    
    if (depth % 1000 == 0) {
        printf("Reached depth: %d\n", depth);
    }
    
    // This will eventually cause stack overflow
    // Commented out to prevent crash
    // test_stack_depth(depth + 1);
}

void stack_depth_demo() {
    printf("\n=== Stack Depth Demo ===\n");
    
    printf("Testing stack depth (safely limited):\n");
    
    // Safe demonstration
    for (int i = 0; i < 10; i++) {
        test_stack_depth(i);
    }
    
    printf("Maximum safe demonstration depth: %d\n", max_depth_reached);
    printf("Note: Actual stack limit is much higher but would crash the program\n");
}

// Main function
int main() {
    printf("RECURSION AND STACK COMPREHENSIVE EXAMPLES\n");
    printf("==========================================\n");
    
    factorial_demo();
    fibonacci_demo();
    array_recursion_demo();
    string_recursion_demo();
    binary_search_demo();
    hanoi_demo();
    permutation_demo();
    subset_demo();
    merge_sort_demo();
    tail_recursion_demo();
    stack_depth_demo();
    
    printf("\nAll demonstrations completed!\n");
    
    return 0;
}