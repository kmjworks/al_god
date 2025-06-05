#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Generic sorting algorithms that work with any data type

// Function pointer type for comparison
typedef int (*compare_func)(const void*, const void*);

// Function pointer type for swap
typedef void (*swap_func)(void*, void*, size_t);

// Generic swap function
void generic_swap(void *a, void *b, size_t size) {
    unsigned char *pa = (unsigned char*)a;
    unsigned char *pb = (unsigned char*)b;
    unsigned char temp;
    
    for (size_t i = 0; i < size; i++) {
        temp = pa[i];
        pa[i] = pb[i];
        pb[i] = temp;
    }
}

// Generic bubble sort
void bubble_sort(void *array, size_t count, size_t size, compare_func compare) {
    unsigned char *arr = (unsigned char*)array;
    bool swapped;
    
    for (size_t i = 0; i < count - 1; i++) {
        swapped = false;
        for (size_t j = 0; j < count - i - 1; j++) {
            void *elem1 = arr + (j * size);
            void *elem2 = arr + ((j + 1) * size);
            
            if (compare(elem1, elem2) > 0) {
                generic_swap(elem1, elem2, size);
                swapped = true;
            }
        }
        
        if (!swapped) break; // Array is sorted
    }
}

// Generic selection sort
void selection_sort(void *array, size_t count, size_t size, compare_func compare) {
    unsigned char *arr = (unsigned char*)array;
    
    for (size_t i = 0; i < count - 1; i++) {
        size_t min_idx = i;
        
        for (size_t j = i + 1; j < count; j++) {
            void *elem_j = arr + (j * size);
            void *elem_min = arr + (min_idx * size);
            
            if (compare(elem_j, elem_min) < 0) {
                min_idx = j;
            }
        }
        
        if (min_idx != i) {
            void *elem_i = arr + (i * size);
            void *elem_min = arr + (min_idx * size);
            generic_swap(elem_i, elem_min, size);
        }
    }
}

// Generic insertion sort
void insertion_sort(void *array, size_t count, size_t size, compare_func compare) {
    unsigned char *arr = (unsigned char*)array;
    unsigned char *temp = (unsigned char*)malloc(size);
    
    if (!temp) return;
    
    for (size_t i = 1; i < count; i++) {
        memcpy(temp, arr + (i * size), size);
        
        size_t j = i;
        while (j > 0 && compare(arr + ((j - 1) * size), temp) > 0) {
            memcpy(arr + (j * size), arr + ((j - 1) * size), size);
            j--;
        }
        
        memcpy(arr + (j * size), temp, size);
    }
    
    free(temp);
}

// Generic merge sort helper
void merge(void *array, size_t left, size_t mid, size_t right, 
           size_t size, compare_func compare) {
    unsigned char *arr = (unsigned char*)array;
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;
    
    unsigned char *L = (unsigned char*)malloc(n1 * size);
    unsigned char *R = (unsigned char*)malloc(n2 * size);
    
    if (!L || !R) {
        free(L);
        free(R);
        return;
    }
    
    // Copy data to temp arrays
    memcpy(L, arr + (left * size), n1 * size);
    memcpy(R, arr + ((mid + 1) * size), n2 * size);
    
    size_t i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (compare(L + (i * size), R + (j * size)) <= 0) {
            memcpy(arr + (k * size), L + (i * size), size);
            i++;
        } else {
            memcpy(arr + (k * size), R + (j * size), size);
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        memcpy(arr + (k * size), L + (i * size), size);
        i++;
        k++;
    }
    
    while (j < n2) {
        memcpy(arr + (k * size), R + (j * size), size);
        j++;
        k++;
    }
    
    free(L);
    free(R);
}

// Generic merge sort recursive helper
void merge_sort_recursive(void *array, size_t left, size_t right, 
                         size_t size, compare_func compare) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        
        merge_sort_recursive(array, left, mid, size, compare);
        merge_sort_recursive(array, mid + 1, right, size, compare);
        merge(array, left, mid, right, size, compare);
    }
}

// Generic merge sort
void merge_sort(void *array, size_t count, size_t size, compare_func compare) {
    if (count > 1) {
        merge_sort_recursive(array, 0, count - 1, size, compare);
    }
}

// Generic quick sort partition
size_t partition(void *array, size_t low, size_t high, 
                size_t size, compare_func compare) {
    unsigned char *arr = (unsigned char*)array;
    unsigned char *pivot = (unsigned char*)malloc(size);
    if (!pivot) return low;
    
    memcpy(pivot, arr + (high * size), size);
    
    size_t i = low;
    
    for (size_t j = low; j < high; j++) {
        if (compare(arr + (j * size), pivot) <= 0) {
            generic_swap(arr + (i * size), arr + (j * size), size);
            i++;
        }
    }
    
    generic_swap(arr + (i * size), arr + (high * size), size);
    free(pivot);
    
    return i;
}

// Generic quick sort recursive helper
void quick_sort_recursive(void *array, size_t low, size_t high, 
                         size_t size, compare_func compare) {
    if (low < high && high != (size_t)-1) {
        size_t pi = partition(array, low, high, size, compare);
        
        if (pi > 0) {
            quick_sort_recursive(array, low, pi - 1, size, compare);
        }
        quick_sort_recursive(array, pi + 1, high, size, compare);
    }
}

// Generic quick sort
void quick_sort(void *array, size_t count, size_t size, compare_func compare) {
    if (count > 1) {
        quick_sort_recursive(array, 0, count - 1, size, compare);
    }
}

// Generic heap sort helpers
void heapify(void *array, size_t n, size_t i, size_t size, compare_func compare) {
    unsigned char *arr = (unsigned char*)array;
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;
    
    if (left < n && compare(arr + (left * size), arr + (largest * size)) > 0) {
        largest = left;
    }
    
    if (right < n && compare(arr + (right * size), arr + (largest * size)) > 0) {
        largest = right;
    }
    
    if (largest != i) {
        generic_swap(arr + (i * size), arr + (largest * size), size);
        heapify(array, n, largest, size, compare);
    }
}

// Generic heap sort
void heap_sort(void *array, size_t count, size_t size, compare_func compare) {
    unsigned char *arr = (unsigned char*)array;
    
    // Build heap
    for (int i = count / 2 - 1; i >= 0; i--) {
        heapify(array, count, i, size, compare);
    }
    
    // Extract elements from heap
    for (size_t i = count - 1; i > 0; i--) {
        generic_swap(arr, arr + (i * size), size);
        heapify(array, i, 0, size, compare);
    }
}

// Comparison functions for different types
int compare_int(const void *a, const void *b) {
    return *(const int*)a - *(const int*)b;
}

int compare_double(const void *a, const void *b) {
    double diff = *(const double*)a - *(const double*)b;
    return (diff > 0) - (diff < 0);
}

int compare_string(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// Structure for demo
typedef struct {
    char name[50];
    int age;
    double salary;
} Person;

int compare_person_by_age(const void *a, const void *b) {
    const Person *p1 = (const Person*)a;
    const Person *p2 = (const Person*)b;
    return p1->age - p2->age;
}

int compare_person_by_salary(const void *a, const void *b) {
    const Person *p1 = (const Person*)a;
    const Person *p2 = (const Person*)b;
    return (p1->salary > p2->salary) - (p1->salary < p2->salary);
}

int compare_person_by_name(const void *a, const void *b) {
    const Person *p1 = (const Person*)a;
    const Person *p2 = (const Person*)b;
    return strcmp(p1->name, p2->name);
}

// Generic print function
void print_array(void *array, size_t count, size_t size, 
                void (*print_elem)(const void*)) {
    unsigned char *arr = (unsigned char*)array;
    
    printf("[");
    for (size_t i = 0; i < count; i++) {
        print_elem(arr + (i * size));
        if (i < count - 1) printf(", ");
    }
    printf("]\n");
}

// Print functions for different types
void print_int(const void *elem) {
    printf("%d", *(const int*)elem);
}

void print_double(const void *elem) {
    printf("%.2f", *(const double*)elem);
}

void print_string(const void *elem) {
    printf("\"%s\"", *(const char**)elem);
}

void print_person(const void *elem) {
    const Person *p = (const Person*)elem;
    printf("{%s, %d, %.0f}", p->name, p->age, p->salary);
}

// Demo function
void generic_sort_demo() {
    printf("=== Generic Sort Demo ===\n\n");
    
    // Test with integers
    printf("1. Integer array sorting:\n");
    int int_array[] = {64, 34, 25, 12, 22, 11, 90};
    size_t int_count = sizeof(int_array) / sizeof(int_array[0]);
    
    printf("Original: ");
    print_array(int_array, int_count, sizeof(int), print_int);
    
    bubble_sort(int_array, int_count, sizeof(int), compare_int);
    printf("Bubble sort: ");
    print_array(int_array, int_count, sizeof(int), print_int);
    
    // Test with doubles
    printf("\n2. Double array sorting:\n");
    double double_array[] = {3.14, 2.71, 1.41, 9.81, 6.67, 2.99};
    size_t double_count = sizeof(double_array) / sizeof(double_array[0]);
    
    printf("Original: ");
    print_array(double_array, double_count, sizeof(double), print_double);
    
    merge_sort(double_array, double_count, sizeof(double), compare_double);
    printf("Merge sort: ");
    print_array(double_array, double_count, sizeof(double), print_double);
    
    // Test with strings
    printf("\n3. String array sorting:\n");
    char *string_array[] = {"zebra", "apple", "mango", "banana", "cherry"};
    size_t string_count = sizeof(string_array) / sizeof(string_array[0]);
    
    printf("Original: ");
    print_array(string_array, string_count, sizeof(char*), print_string);
    
    quick_sort(string_array, string_count, sizeof(char*), compare_string);
    printf("Quick sort: ");
    print_array(string_array, string_count, sizeof(char*), print_string);
    
    // Test with structures
    printf("\n4. Structure array sorting:\n");
    Person people[] = {
        {"Alice", 30, 50000},
        {"Bob", 25, 60000},
        {"Charlie", 35, 55000},
        {"David", 28, 58000},
        {"Eve", 32, 52000}
    };
    size_t people_count = sizeof(people) / sizeof(people[0]);
    
    printf("Original: ");
    print_array(people, people_count, sizeof(Person), print_person);
    
    insertion_sort(people, people_count, sizeof(Person), compare_person_by_age);
    printf("Sort by age: ");
    print_array(people, people_count, sizeof(Person), print_person);
    
    heap_sort(people, people_count, sizeof(Person), compare_person_by_salary);
    printf("Sort by salary: ");
    print_array(people, people_count, sizeof(Person), print_person);
    
    selection_sort(people, people_count, sizeof(Person), compare_person_by_name);
    printf("Sort by name: ");
    print_array(people, people_count, sizeof(Person), print_person);
    
    // Performance comparison
    printf("\n5. Performance comparison (sorting 1000 random integers):\n");
    
    int *test_array = (int*)malloc(1000 * sizeof(int));
    int *work_array = (int*)malloc(1000 * sizeof(int));
    
    // Generate random data
    for (int i = 0; i < 1000; i++) {
        test_array[i] = rand() % 1000;
    }
    
    // Test each algorithm
    clock_t start, end;
    
    memcpy(work_array, test_array, 1000 * sizeof(int));
    start = clock();
    bubble_sort(work_array, 1000, sizeof(int), compare_int);
    end = clock();
    printf("Bubble sort: %.6f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    memcpy(work_array, test_array, 1000 * sizeof(int));
    start = clock();
    selection_sort(work_array, 1000, sizeof(int), compare_int);
    end = clock();
    printf("Selection sort: %.6f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    memcpy(work_array, test_array, 1000 * sizeof(int));
    start = clock();
    insertion_sort(work_array, 1000, sizeof(int), compare_int);
    end = clock();
    printf("Insertion sort: %.6f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    memcpy(work_array, test_array, 1000 * sizeof(int));
    start = clock();
    merge_sort(work_array, 1000, sizeof(int), compare_int);
    end = clock();
    printf("Merge sort: %.6f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    memcpy(work_array, test_array, 1000 * sizeof(int));
    start = clock();
    quick_sort(work_array, 1000, sizeof(int), compare_int);
    end = clock();
    printf("Quick sort: %.6f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    memcpy(work_array, test_array, 1000 * sizeof(int));
    start = clock();
    heap_sort(work_array, 1000, sizeof(int), compare_int);
    end = clock();
    printf("Heap sort: %.6f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    free(test_array);
    free(work_array);
}

int main() {
    generic_sort_demo();
    return 0;
}