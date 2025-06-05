#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generic bubble sort using void pointers and function pointers
void generic_bubble_sort(void *base, size_t num_elements, size_t element_size,
                        int (*compare)(const void *, const void *)) {
    char *array = (char *)base;
    void *temp = malloc(element_size);
    
    for (size_t i = 0; i < num_elements - 1; i++) {
        for (size_t j = 0; j < num_elements - i - 1; j++) {
            void *elem1 = array + (j * element_size);
            void *elem2 = array + ((j + 1) * element_size);
            
            if (compare(elem1, elem2) > 0) {
                // Swap elements
                memcpy(temp, elem1, element_size);
                memcpy(elem1, elem2, element_size);
                memcpy(elem2, temp, element_size);
            }
        }
    }
    
    free(temp);
}

// Comparison functions for different types
int compare_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int compare_double(const void *a, const void *b) {
    double diff = *(double *)a - *(double *)b;
    return (diff > 0) - (diff < 0);
}

int compare_string(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

// Custom struct example
typedef struct {
    int id;
    char name[50];
    double score;
} Student;

int compare_student_by_score(const void *a, const void *b) {
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;
    double diff = s1->score - s2->score;
    return (diff > 0) - (diff < 0);
}

int compare_student_by_name(const void *a, const void *b) {
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;
    return strcmp(s1->name, s2->name);
}

// Example usage
int main() {
    // Example 1: Sorting integers
    int numbers[] = {64, 34, 25, 12, 22, 11, 90};
    size_t n = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("Original integers: ");
    for (size_t i = 0; i < n; i++) printf("%d ", numbers[i]);
    printf("\n");
    
    generic_bubble_sort(numbers, n, sizeof(int), compare_int);
    
    printf("Sorted integers: ");
    for (size_t i = 0; i < n; i++) printf("%d ", numbers[i]);
    printf("\n\n");
    
    // Example 2: Sorting doubles
    double doubles[] = {3.14, 2.71, 1.41, 0.57, 2.22};
    size_t d = sizeof(doubles) / sizeof(doubles[0]);
    
    printf("Original doubles: ");
    for (size_t i = 0; i < d; i++) printf("%.2f ", doubles[i]);
    printf("\n");
    
    generic_bubble_sort(doubles, d, sizeof(double), compare_double);
    
    printf("Sorted doubles: ");
    for (size_t i = 0; i < d; i++) printf("%.2f ", doubles[i]);
    printf("\n\n");
    
    // Example 3: Sorting strings
    char *strings[] = {"zebra", "apple", "mango", "banana", "cherry"};
    size_t s = sizeof(strings) / sizeof(strings[0]);
    
    printf("Original strings: ");
    for (size_t i = 0; i < s; i++) printf("%s ", strings[i]);
    printf("\n");
    
    generic_bubble_sort(strings, s, sizeof(char *), compare_string);
    
    printf("Sorted strings: ");
    for (size_t i = 0; i < s; i++) printf("%s ", strings[i]);
    printf("\n\n");
    
    // Example 4: Sorting structs
    Student students[] = {
        {1, "Alice", 85.5},
        {2, "Bob", 92.3},
        {3, "Charlie", 78.9},
        {4, "David", 88.7}
    };
    size_t st = sizeof(students) / sizeof(students[0]);
    
    printf("Students by score:\n");
    generic_bubble_sort(students, st, sizeof(Student), compare_student_by_score);
    for (size_t i = 0; i < st; i++) {
        printf("ID: %d, Name: %s, Score: %.1f\n", 
               students[i].id, students[i].name, students[i].score);
    }
    
    printf("\nStudents by name:\n");
    generic_bubble_sort(students, st, sizeof(Student), compare_student_by_name);
    for (size_t i = 0; i < st; i++) {
        printf("ID: %d, Name: %s, Score: %.1f\n", 
               students[i].id, students[i].name, students[i].score);
    }
    
    return 0;
}