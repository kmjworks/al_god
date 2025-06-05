#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Structure definitions
typedef struct {
    char name[50];
    int age;
    float gpa;
    int id;
} Student;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    char title[100];
    char author[50];
    int year;
    float price;
} Book;

// Integer comparison functions
int compare_int_asc(const void *a, const void *b) {
    return *(const int*)a - *(const int*)b;
}

int compare_int_desc(const void *a, const void *b) {
    return *(const int*)b - *(const int*)a;
}

// Double comparison (safe)
int compare_double(const void *a, const void *b) {
    double diff = *(const double*)a - *(const double*)b;
    return (diff > 0) - (diff < 0);
}

// String comparison
int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

int compare_strings_desc(const void *a, const void *b) {
    return strcmp(*(const char**)b, *(const char**)a);
}

// Character array comparison
int compare_char_arrays(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

// Student comparison functions
int compare_student_by_name(const void *a, const void *b) {
    const Student *s1 = (const Student*)a;
    const Student *s2 = (const Student*)b;
    return strcmp(s1->name, s2->name);
}

int compare_student_by_age(const void *a, const void *b) {
    const Student *s1 = (const Student*)a;
    const Student *s2 = (const Student*)b;
    return s1->age - s2->age;
}

int compare_student_by_gpa_desc(const void *a, const void *b) {
    const Student *s1 = (const Student*)a;
    const Student *s2 = (const Student*)b;
    return (s2->gpa > s1->gpa) - (s2->gpa < s1->gpa);
}

// Multi-level sort: primary by GPA (desc), secondary by name (asc)
int compare_student_multi(const void *a, const void *b) {
    const Student *s1 = (const Student*)a;
    const Student *s2 = (const Student*)b;
    
    // Compare GPA (descending)
    if (s1->gpa != s2->gpa) {
        return (s2->gpa > s1->gpa) - (s2->gpa < s1->gpa);
    }
    
    // If GPA is equal, compare by name
    return strcmp(s1->name, s2->name);
}

// Point comparison by distance from origin
int compare_points_by_distance(const void *a, const void *b) {
    const Point *p1 = (const Point*)a;
    const Point *p2 = (const Point*)b;
    
    double dist1 = sqrt(p1->x * p1->x + p1->y * p1->y);
    double dist2 = sqrt(p2->x * p2->x + p2->y * p2->y);
    
    return (dist1 > dist2) - (dist1 < dist2);
}

// Book comparison by year then title
int compare_books(const void *a, const void *b) {
    const Book *b1 = (const Book*)a;
    const Book *b2 = (const Book*)b;
    
    if (b1->year != b2->year) {
        return b1->year - b2->year;
    }
    return strcmp(b1->title, b2->title);
}

// Demonstration functions
void demo_integer_sorting() {
    printf("\n=== Integer Sorting Demo ===\n");
    
    int numbers[] = {42, 17, 93, 5, 68, 31, 77, 12, 89, 24};
    int n = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("Original: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // Sort ascending
    qsort(numbers, n, sizeof(int), compare_int_asc);
    printf("Ascending: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // Sort descending
    qsort(numbers, n, sizeof(int), compare_int_desc);
    printf("Descending: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
}

void demo_double_sorting() {
    printf("\n=== Double Sorting Demo ===\n");
    
    double values[] = {3.14, 2.71, 1.41, 9.81, 6.67, 2.99};
    int n = sizeof(values) / sizeof(values[0]);
    
    printf("Original: ");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", values[i]);
    }
    printf("\n");
    
    qsort(values, n, sizeof(double), compare_double);
    printf("Sorted: ");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", values[i]);
    }
    printf("\n");
}

void demo_string_sorting() {
    printf("\n=== String Sorting Demo ===\n");
    
    // Array of string pointers
    char *words[] = {"zebra", "apple", "mango", "banana", "cherry", "date"};
    int n = sizeof(words) / sizeof(words[0]);
    
    printf("Original: ");
    for (int i = 0; i < n; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    qsort(words, n, sizeof(char*), compare_strings);
    printf("Sorted (asc): ");
    for (int i = 0; i < n; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    // Array of character arrays
    char names[][20] = {"John", "Alice", "Bob", "Charlie", "David", "Eve"};
    n = sizeof(names) / sizeof(names[0]);
    
    printf("\nCharacter arrays:\n");
    printf("Original: ");
    for (int i = 0; i < n; i++) {
        printf("%s ", names[i]);
    }
    printf("\n");
    
    qsort(names, n, sizeof(names[0]), compare_char_arrays);
    printf("Sorted: ");
    for (int i = 0; i < n; i++) {
        printf("%s ", names[i]);
    }
    printf("\n");
}

void demo_struct_sorting() {
    printf("\n=== Structure Sorting Demo ===\n");
    
    Student students[] = {
        {"Alice", 20, 3.8, 1001},
        {"Bob", 19, 3.5, 1002},
        {"Charlie", 21, 3.8, 1003},
        {"David", 20, 3.2, 1004},
        {"Eve", 19, 3.9, 1005}
    };
    int n = sizeof(students) / sizeof(students[0]);
    
    printf("Original order:\n");
    for (int i = 0; i < n; i++) {
        printf("%-10s Age: %d, GPA: %.1f\n", 
               students[i].name, students[i].age, students[i].gpa);
    }
    
    // Sort by name
    qsort(students, n, sizeof(Student), compare_student_by_name);
    printf("\nSorted by name:\n");
    for (int i = 0; i < n; i++) {
        printf("%-10s Age: %d, GPA: %.1f\n", 
               students[i].name, students[i].age, students[i].gpa);
    }
    
    // Sort by GPA (descending)
    qsort(students, n, sizeof(Student), compare_student_by_gpa_desc);
    printf("\nSorted by GPA (descending):\n");
    for (int i = 0; i < n; i++) {
        printf("%-10s Age: %d, GPA: %.1f\n", 
               students[i].name, students[i].age, students[i].gpa);
    }
    
    // Multi-level sort
    qsort(students, n, sizeof(Student), compare_student_multi);
    printf("\nMulti-level sort (GPA desc, then name):\n");
    for (int i = 0; i < n; i++) {
        printf("%-10s Age: %d, GPA: %.1f\n", 
               students[i].name, students[i].age, students[i].gpa);
    }
}

void demo_point_sorting() {
    printf("\n=== Point Sorting Demo ===\n");
    
    Point points[] = {{3, 4}, {1, 1}, {5, 0}, {2, 2}, {0, 5}};
    int n = sizeof(points) / sizeof(points[0]);
    
    printf("Original points:\n");
    for (int i = 0; i < n; i++) {
        double dist = sqrt(points[i].x * points[i].x + points[i].y * points[i].y);
        printf("(%d, %d) - distance: %.2f\n", points[i].x, points[i].y, dist);
    }
    
    qsort(points, n, sizeof(Point), compare_points_by_distance);
    printf("\nSorted by distance from origin:\n");
    for (int i = 0; i < n; i++) {
        double dist = sqrt(points[i].x * points[i].x + points[i].y * points[i].y);
        printf("(%d, %d) - distance: %.2f\n", points[i].x, points[i].y, dist);
    }
}

// Binary search example
void demo_bsearch() {
    printf("\n=== Binary Search Demo ===\n");
    
    int numbers[] = {5, 12, 17, 24, 31, 42, 68, 77, 89, 93};
    int n = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    int keys[] = {42, 100, 5, 93, 50};
    for (int i = 0; i < 5; i++) {
        int *found = (int*)bsearch(&keys[i], numbers, n, sizeof(int), compare_int_asc);
        if (found) {
            printf("Found %d at index %ld\n", keys[i], found - numbers);
        } else {
            printf("%d not found\n", keys[i]);
        }
    }
}

// Demonstrate library functions
void demo_library_functions() {
    printf("\n=== Library Functions Demo ===\n");
    
    // Random numbers
    srand(time(NULL));
    printf("Random numbers: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", rand() % 100);
    }
    printf("\n");
    
    // String conversion
    char *num_str = "12345";
    int num = atoi(num_str);
    printf("atoi(\"%s\") = %d\n", num_str, num);
    
    char *float_str = "3.14159";
    double fnum = atof(float_str);
    printf("atof(\"%s\") = %f\n", float_str, fnum);
    
    // String operations
    char str1[50] = "Hello";
    char str2[] = " World";
    strcat(str1, str2);
    printf("strcat result: %s\n", str1);
    
    // Math operations
    printf("sqrt(16) = %.0f\n", sqrt(16));
    printf("pow(2, 10) = %.0f\n", pow(2, 10));
    printf("ceil(3.14) = %.0f\n", ceil(3.14));
    printf("floor(3.14) = %.0f\n", floor(3.14));
}

int main() {
    printf("QSORT AND LIBRARIES COMPREHENSIVE EXAMPLES\n");
    printf("==========================================\n");
    
    demo_integer_sorting();
    demo_double_sorting();
    demo_string_sorting();
    demo_struct_sorting();
    demo_point_sorting();
    demo_bsearch();
    demo_library_functions();
    
    return 0;
}