# Recursion and Stack Cheat Sheet - C Programming

## What is Recursion?
Recursion is a programming technique where a function calls itself to solve a problem by breaking it down into smaller, similar subproblems.

## Key Components of Recursion
1. **Base Case**: The condition that stops the recursion
2. **Recursive Case**: The function calling itself with modified parameters
3. **Progress**: Each recursive call must move toward the base case

## How the Stack Works

### Call Stack Structure
```
High Memory
┌─────────────────┐
│ main() frame    │
├─────────────────┤
│ func1() frame   │
├─────────────────┤
│ func2() frame   │ ← Stack Pointer (SP)
├─────────────────┤
│ (unused)        │
│                 │
└─────────────────┘
Low Memory
```

### Stack Frame Contents
Each function call creates a stack frame containing:
- Return address
- Function parameters
- Local variables
- Saved registers

## Basic Recursion Examples

### 1. Factorial
```c
// Recursive approach
int factorial(int n) {
    // Base case
    if (n <= 1) {
        return 1;
    }
    // Recursive case
    return n * factorial(n - 1);
}

// Trace for factorial(4):
// factorial(4) = 4 * factorial(3)
// factorial(3) = 3 * factorial(2)
// factorial(2) = 2 * factorial(1)
// factorial(1) = 1 (base case)
// Unwind: 2*1=2, 3*2=6, 4*6=24

// Iterative version for comparison
int factorial_iterative(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}
```

### 2. Fibonacci Sequence
```c
// Simple recursive (inefficient)
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Optimized with memoization
int fibonacci_memo(int n, int* cache) {
    if (n <= 1) {
        return n;
    }
    if (cache[n] != -1) {
        return cache[n];
    }
    cache[n] = fibonacci_memo(n - 1, cache) + fibonacci_memo(n - 2, cache);
    return cache[n];
}

// Tail recursive version
int fib_tail_helper(int n, int a, int b) {
    if (n == 0) return a;
    if (n == 1) return b;
    return fib_tail_helper(n - 1, b, a + b);
}

int fibonacci_tail(int n) {
    return fib_tail_helper(n, 0, 1);
}
```

### 3. Power Function
```c
// Basic recursive power
double power(double base, int exp) {
    if (exp == 0) {
        return 1.0;
    }
    if (exp < 0) {
        return 1.0 / power(base, -exp);
    }
    return base * power(base, exp - 1);
}

// Optimized power (O(log n))
double power_fast(double base, int exp) {
    if (exp == 0) return 1.0;
    if (exp < 0) return 1.0 / power_fast(base, -exp);
    
    if (exp % 2 == 0) {
        double half = power_fast(base, exp / 2);
        return half * half;
    } else {
        return base * power_fast(base, exp - 1);
    }
}
```

## Array and String Recursion

### 4. Array Sum
```c
// Sum array elements recursively
int array_sum(int arr[], int n) {
    if (n <= 0) {
        return 0;
    }
    return arr[n - 1] + array_sum(arr, n - 1);
}

// Alternative: sum from start to end
int array_sum_range(int arr[], int start, int end) {
    if (start > end) {
        return 0;
    }
    if (start == end) {
        return arr[start];
    }
    int mid = (start + end) / 2;
    return array_sum_range(arr, start, mid) + 
           array_sum_range(arr, mid + 1, end);
}
```

### 5. String Reversal
```c
// Reverse string in place
void reverse_string(char* str, int start, int end) {
    if (start >= end) {
        return;
    }
    // Swap characters
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    
    // Recurse for inner substring
    reverse_string(str, start + 1, end - 1);
}

// Check palindrome
bool is_palindrome(char* str, int start, int end) {
    if (start >= end) {
        return true;
    }
    if (str[start] != str[end]) {
        return false;
    }
    return is_palindrome(str, start + 1, end - 1);
}
```

### 6. Binary Search
```c
int binary_search(int arr[], int left, int right, int target) {
    if (left > right) {
        return -1;  // Not found
    }
    
    int mid = left + (right - left) / 2;
    
    if (arr[mid] == target) {
        return mid;
    }
    
    if (arr[mid] > target) {
        return binary_search(arr, left, mid - 1, target);
    } else {
        return binary_search(arr, mid + 1, right, target);
    }
}
```

## Advanced Recursion Patterns

### 7. Divide and Conquer - Merge Sort
```c
void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Create temp arrays
    int L[n1], R[n2];
    
    // Copy data
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    // Merge back
    i = 0; j = 0; k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy remaining
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        // Sort first and second halves
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        
        // Merge sorted halves
        merge(arr, left, mid, right);
    }
}
```

### 8. Quick Sort
```c
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return (i + 1);
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}
```

### 9. Tower of Hanoi
```c
void tower_of_hanoi(int n, char from, char to, char aux) {
    if (n == 1) {
        printf("Move disk 1 from %c to %c\n", from, to);
        return;
    }
    
    // Move n-1 disks from source to auxiliary
    tower_of_hanoi(n - 1, from, aux, to);
    
    // Move largest disk from source to destination
    printf("Move disk %d from %c to %c\n", n, from, to);
    
    // Move n-1 disks from auxiliary to destination
    tower_of_hanoi(n - 1, aux, to, from);
}
```

## Backtracking Recursion

### 10. Generate All Permutations
```c
void swap(char* a, char* b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

void permute(char* str, int left, int right) {
    if (left == right) {
        printf("%s\n", str);
        return;
    }
    
    for (int i = left; i <= right; i++) {
        swap(&str[left], &str[i]);
        permute(str, left + 1, right);
        swap(&str[left], &str[i]);  // Backtrack
    }
}
```

### 11. N-Queens Problem
```c
#define N 8

bool is_safe(int board[N][N], int row, int col) {
    // Check row on left
    for (int i = 0; i < col; i++) {
        if (board[row][i]) return false;
    }
    
    // Check upper diagonal
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j]) return false;
    }
    
    // Check lower diagonal
    for (int i = row, j = col; j >= 0 && i < N; i++, j--) {
        if (board[i][j]) return false;
    }
    
    return true;
}

bool solve_n_queens(int board[N][N], int col) {
    if (col >= N) {
        return true;  // All queens placed
    }
    
    for (int i = 0; i < N; i++) {
        if (is_safe(board, i, col)) {
            board[i][col] = 1;
            
            if (solve_n_queens(board, col + 1)) {
                return true;
            }
            
            board[i][col] = 0;  // Backtrack
        }
    }
    
    return false;
}
```

### 12. Subset Sum
```c
bool subset_sum(int set[], int n, int sum) {
    // Base cases
    if (sum == 0) return true;
    if (n == 0) return false;
    
    // If last element is greater than sum, ignore it
    if (set[n - 1] > sum) {
        return subset_sum(set, n - 1, sum);
    }
    
    // Check if sum can be obtained by:
    // 1. Including the last element
    // 2. Excluding the last element
    return subset_sum(set, n - 1, sum) || 
           subset_sum(set, n - 1, sum - set[n - 1]);
}
```

## Tree Recursion

### 13. Binary Tree Traversals
```c
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Preorder: Root -> Left -> Right
void preorder(TreeNode* root) {
    if (root == NULL) return;
    
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

// Inorder: Left -> Root -> Right
void inorder(TreeNode* root) {
    if (root == NULL) return;
    
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

// Postorder: Left -> Right -> Root
void postorder(TreeNode* root) {
    if (root == NULL) return;
    
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

// Calculate tree height
int tree_height(TreeNode* root) {
    if (root == NULL) return 0;
    
    int left_height = tree_height(root->left);
    int right_height = tree_height(root->right);
    
    return 1 + (left_height > right_height ? left_height : right_height);
}
```

## Tail Recursion Optimization

### 14. Tail Recursive Examples
```c
// Regular recursion (not tail recursive)
int sum_to_n(int n) {
    if (n <= 0) return 0;
    return n + sum_to_n(n - 1);  // Operation after recursive call
}

// Tail recursive version
int sum_to_n_tail(int n, int accumulator) {
    if (n <= 0) return accumulator;
    return sum_to_n_tail(n - 1, accumulator + n);  // Nothing after call
}

// Tail recursive factorial
int factorial_tail(int n, int accumulator) {
    if (n <= 1) return accumulator;
    return factorial_tail(n - 1, n * accumulator);
}

// Tail recursive GCD
int gcd_tail(int a, int b) {
    if (b == 0) return a;
    return gcd_tail(b, a % b);  // Tail position
}
```

## Stack Overflow Prevention

### 15. Converting Recursion to Iteration
```c
// Recursive fibonacci (stack intensive)
int fib_recursive(int n) {
    if (n <= 1) return n;
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

// Iterative fibonacci (O(1) space)
int fib_iterative(int n) {
    if (n <= 1) return n;
    
    int prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

// Using explicit stack for recursion
typedef struct {
    int* items;
    int top;
    int capacity;
} Stack;

void dfs_iterative(TreeNode* root) {
    if (root == NULL) return;
    
    Stack stack = {
        .items = malloc(100 * sizeof(int)),
        .top = -1,
        .capacity = 100
    };
    
    // Push root
    stack.items[++stack.top] = (intptr_t)root;
    
    while (stack.top >= 0) {
        TreeNode* node = (TreeNode*)stack.items[stack.top--];
        printf("%d ", node->data);
        
        // Push children (right first for left-to-right order)
        if (node->right) {
            stack.items[++stack.top] = (intptr_t)node->right;
        }
        if (node->left) {
            stack.items[++stack.top] = (intptr_t)node->left;
        }
    }
    
    free(stack.items);
}
```

## Mutual Recursion

### 16. Even and Odd Functions
```c
// Forward declaration
bool is_even(int n);

bool is_odd(int n) {
    if (n == 0) return false;
    return is_even(n - 1);
}

bool is_even(int n) {
    if (n == 0) return true;
    return is_odd(n - 1);
}
```

## Common Pitfalls and Solutions

### 1. Missing Base Case
```c
// BAD: No base case
int bad_factorial(int n) {
    return n * bad_factorial(n - 1);  // Infinite recursion!
}

// GOOD: Has base case
int good_factorial(int n) {
    if (n <= 1) return 1;  // Base case
    return n * good_factorial(n - 1);
}
```

### 2. Stack Overflow
```c
// BAD: Deep recursion
int sum_large(int n) {
    if (n == 0) return 0;
    return n + sum_large(n - 1);  // Stack overflow for large n
}

// GOOD: Use iteration for simple cases
int sum_large_safe(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += i;
    }
    return sum;
}
```

### 3. Redundant Calculations
```c
// BAD: Exponential time complexity
int fib_slow(int n) {
    if (n <= 1) return n;
    return fib_slow(n - 1) + fib_slow(n - 2);  // Many repeated calls
}

// GOOD: Memoization
int fib_memo[100] = {0};
int fib_fast(int n) {
    if (n <= 1) return n;
    if (fib_memo[n] != 0) return fib_memo[n];
    
    fib_memo[n] = fib_fast(n - 1) + fib_fast(n - 2);
    return fib_memo[n];
}
```

## Recursion Depth Analysis

### Stack Space Complexity
- **O(n)**: Linear recursion (factorial, sum)
- **O(log n)**: Binary recursion with divide and conquer
- **O(n)**: Tree traversal (worst case)
- **O(2^n)**: Exponential recursion (naive fibonacci)

### Maximum Recursion Depth
```c
// Test maximum recursion depth
int max_depth = 0;

void test_depth(int depth) {
    max_depth = depth;
    printf("Depth: %d\n", depth);
    test_depth(depth + 1);  // Will eventually crash
}

// Typical limits:
// - Default stack size: 1-8 MB
// - Each frame: ~100-1000 bytes
// - Max depth: ~1000-10000 calls
```

## Best Practices

1. **Always define a base case**
2. **Ensure progress toward base case**
3. **Consider iterative solutions for simple problems**
4. **Use memoization for overlapping subproblems**
5. **Be aware of stack limitations**
6. **Prefer tail recursion when possible**
7. **Test with small inputs first**
8. **Consider space complexity**

## Quick Reference

| Problem Type | Recursive Approach | Time | Space |
|--------------|-------------------|------|-------|
| Linear search | Check first, recurse on rest | O(n) | O(n) |
| Binary search | Divide in half | O(log n) | O(log n) |
| Tree traversal | Visit node, recurse on children | O(n) | O(h) |
| Factorial | n * factorial(n-1) | O(n) | O(n) |
| Fibonacci | fib(n-1) + fib(n-2) | O(2^n) | O(n) |
| Merge sort | Divide, sort, merge | O(n log n) | O(n) |

## Exam Tips
1. Identify base case and recursive case
2. Trace through small examples by hand
3. Draw the recursion tree for complex problems
4. Calculate time and space complexity
5. Consider whether recursion is the best approach
6. Watch for stack overflow possibilities
7. Understand tail recursion optimization
8. Practice converting between recursive and iterative