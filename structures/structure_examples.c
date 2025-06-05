#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Basic structure
struct Point {
    int x;
    int y;
};

// Structure with typedef
typedef struct {
    char name[50];
    int age;
    float salary;
} Employee;

// Nested structures
typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char title[100];
    char author[50];
    Date publishDate;
    float price;
    int pages;
} Book;

// Self-referential structure
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Structure with arrays
typedef struct {
    char name[50];
    int scores[5];
    float average;
} Student;

// Bit fields
typedef struct {
    unsigned int isActive : 1;
    unsigned int priority : 3;
    unsigned int type : 4;
    unsigned int reserved : 24;
} StatusFlags;

// Union within structure
typedef struct {
    char type;  // 'i', 'f', 's'
    union {
        int intValue;
        float floatValue;
        char stringValue[50];
    } data;
} Variant;

// Complex structure
typedef struct {
    int id;
    char *dynamicName;
    struct {
        char street[100];
        char city[50];
        int zipCode;
    } address;
    Date *importantDates;
    int numDates;
} ComplexRecord;

// Basic structure operations
void demonstrateBasicStructure() {
    printf("\n=== Basic Structure Demo ===\n");
    
    struct Point p1 = {10, 20};
    struct Point p2;
    p2.x = 30;
    p2.y = 40;
    
    printf("Point 1: (%d, %d)\n", p1.x, p1.y);
    printf("Point 2: (%d, %d)\n", p2.x, p2.y);
    
    // Structure assignment
    struct Point p3 = p1;
    printf("Point 3 (copy of p1): (%d, %d)\n", p3.x, p3.y);
    
    // Pointer to structure
    struct Point *ptr = &p1;
    printf("Via pointer: (%d, %d)\n", ptr->x, ptr->y);
    ptr->x = 50;
    printf("Modified p1: (%d, %d)\n", p1.x, p1.y);
}

// Function using structures
float calculateDistance(struct Point p1, struct Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

struct Point midpoint(struct Point p1, struct Point p2) {
    struct Point mid = {
        (p1.x + p2.x) / 2,
        (p1.y + p2.y) / 2
    };
    return mid;
}

void demonstrateFunctionsWithStructures() {
    printf("\n=== Functions with Structures Demo ===\n");
    
    struct Point p1 = {0, 0};
    struct Point p2 = {3, 4};
    
    printf("Distance between points: %.2f\n", calculateDistance(p1, p2));
    
    struct Point mid = midpoint(p1, p2);
    printf("Midpoint: (%d, %d)\n", mid.x, mid.y);
}

// Array of structures
void demonstrateArrayOfStructures() {
    printf("\n=== Array of Structures Demo ===\n");
    
    Employee employees[3] = {
        {"Alice", 25, 50000},
        {"Bob", 30, 60000},
        {"Charlie", 35, 70000}
    };
    
    // Calculate average salary
    float totalSalary = 0;
    for (int i = 0; i < 3; i++) {
        printf("%s: Age %d, Salary $%.2f\n", 
               employees[i].name, employees[i].age, employees[i].salary);
        totalSalary += employees[i].salary;
    }
    printf("Average salary: $%.2f\n", totalSalary / 3);
    
    // Find oldest employee
    int oldestIndex = 0;
    for (int i = 1; i < 3; i++) {
        if (employees[i].age > employees[oldestIndex].age) {
            oldestIndex = i;
        }
    }
    printf("Oldest employee: %s\n", employees[oldestIndex].name);
}

// Dynamic structure allocation
void demonstrateDynamicStructures() {
    printf("\n=== Dynamic Structures Demo ===\n");
    
    // Single structure
    Employee *emp = (Employee*)malloc(sizeof(Employee));
    strcpy(emp->name, "Dynamic Employee");
    emp->age = 28;
    emp->salary = 55000;
    
    printf("Dynamic employee: %s, Age %d\n", emp->name, emp->age);
    
    // Dynamic array of structures
    int n = 5;
    Student *students = (Student*)malloc(n * sizeof(Student));
    
    for (int i = 0; i < n; i++) {
        sprintf(students[i].name, "Student%d", i + 1);
        float total = 0;
        for (int j = 0; j < 5; j++) {
            students[i].scores[j] = 70 + rand() % 31;  // 70-100
            total += students[i].scores[j];
        }
        students[i].average = total / 5;
    }
    
    printf("\nStudent Scores:\n");
    for (int i = 0; i < n; i++) {
        printf("%s: Average = %.1f\n", students[i].name, students[i].average);
    }
    
    free(students);
    free(emp);
}

// Nested structures demo
void demonstrateNestedStructures() {
    printf("\n=== Nested Structures Demo ===\n");
    
    Book book = {
        "The C Programming Language",
        "Kernighan & Ritchie",
        {1, 1, 1978},
        29.99,
        272
    };
    
    printf("Book: %s\n", book.title);
    printf("Author: %s\n", book.author);
    printf("Published: %02d/%02d/%04d\n", 
           book.publishDate.month, book.publishDate.day, book.publishDate.year);
    printf("Price: $%.2f\n", book.price);
    printf("Pages: %d\n", book.pages);
}

// Linked list operations
Node* createNode(int data) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void printList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void freeList(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void demonstrateLinkedList() {
    printf("\n=== Linked List Demo ===\n");
    
    Node *head = createNode(10);
    head->next = createNode(20);
    head->next->next = createNode(30);
    
    printf("Linked list: ");
    printList(head);
    
    freeList(head);
}

// Bit fields demo
void demonstrateBitFields() {
    printf("\n=== Bit Fields Demo ===\n");
    
    StatusFlags status = {0};
    printf("Initial size of StatusFlags: %zu bytes\n", sizeof(StatusFlags));
    
    status.isActive = 1;
    status.priority = 5;  // 0-7 range
    status.type = 10;     // 0-15 range
    
    printf("Status: Active=%d, Priority=%d, Type=%d\n",
           status.isActive, status.priority, status.type);
    
    // Show bit manipulation
    unsigned int *rawData = (unsigned int*)&status;
    printf("Raw data: 0x%08X\n", *rawData);
}

// Union in structure demo
void demonstrateUnionInStructure() {
    printf("\n=== Union in Structure Demo ===\n");
    
    Variant v1 = {'i', .data.intValue = 42};
    Variant v2 = {'f', .data.floatValue = 3.14};
    Variant v3 = {'s'};
    strcpy(v3.data.stringValue, "Hello");
    
    Variant values[] = {v1, v2, v3};
    
    for (int i = 0; i < 3; i++) {
        printf("Value %d: ", i + 1);
        switch (values[i].type) {
            case 'i':
                printf("Integer = %d\n", values[i].data.intValue);
                break;
            case 'f':
                printf("Float = %.2f\n", values[i].data.floatValue);
                break;
            case 's':
                printf("String = %s\n", values[i].data.stringValue);
                break;
        }
    }
}

// Structure padding demo
void demonstrateStructurePadding() {
    printf("\n=== Structure Padding Demo ===\n");
    
    struct PaddedStruct {
        char a;     // 1 byte
        int b;      // 4 bytes
        char c;     // 1 byte
        double d;   // 8 bytes
    };
    
    struct OptimizedStruct {
        double d;   // 8 bytes
        int b;      // 4 bytes
        char a;     // 1 byte
        char c;     // 1 byte
    };
    
    printf("Padded struct size: %zu bytes\n", sizeof(struct PaddedStruct));
    printf("Optimized struct size: %zu bytes\n", sizeof(struct OptimizedStruct));
    
    // Show member offsets
    struct PaddedStruct ps;
    printf("\nPadded struct offsets:\n");
    printf("a: %ld\n", (char*)&ps.a - (char*)&ps);
    printf("b: %ld\n", (char*)&ps.b - (char*)&ps);
    printf("c: %ld\n", (char*)&ps.c - (char*)&ps);
    printf("d: %ld\n", (char*)&ps.d - (char*)&ps);
}

// Complex structure with dynamic members
void demonstrateComplexStructure() {
    printf("\n=== Complex Structure Demo ===\n");
    
    ComplexRecord record;
    record.id = 1001;
    
    // Allocate dynamic name
    record.dynamicName = (char*)malloc(50 * sizeof(char));
    strcpy(record.dynamicName, "Complex Record Example");
    
    // Set address
    strcpy(record.address.street, "123 Main St");
    strcpy(record.address.city, "Anytown");
    record.address.zipCode = 12345;
    
    // Allocate dynamic dates array
    record.numDates = 3;
    record.importantDates = (Date*)malloc(record.numDates * sizeof(Date));
    record.importantDates[0] = (Date){15, 6, 2020};
    record.importantDates[1] = (Date){1, 1, 2021};
    record.importantDates[2] = (Date){25, 12, 2021};
    
    // Display
    printf("ID: %d\n", record.id);
    printf("Name: %s\n", record.dynamicName);
    printf("Address: %s, %s %d\n", 
           record.address.street, record.address.city, record.address.zipCode);
    printf("Important dates:\n");
    for (int i = 0; i < record.numDates; i++) {
        printf("  %02d/%02d/%04d\n", 
               record.importantDates[i].month,
               record.importantDates[i].day,
               record.importantDates[i].year);
    }
    
    // Cleanup
    free(record.dynamicName);
    free(record.importantDates);
}

// Structure copy semantics
void demonstrateStructureCopy() {
    printf("\n=== Structure Copy Demo ===\n");
    
    typedef struct {
        int value;
        int *ptr;
    } Container;
    
    int x = 100;
    Container c1 = {10, &x};
    Container c2 = c1;  // Shallow copy
    
    printf("Original: c1.value=%d, *c1.ptr=%d\n", c1.value, *c1.ptr);
    printf("Copy: c2.value=%d, *c2.ptr=%d\n", c2.value, *c2.ptr);
    
    // Modify through c2
    c2.value = 20;
    *c2.ptr = 200;
    
    printf("\nAfter modification:\n");
    printf("c1.value=%d, *c1.ptr=%d\n", c1.value, *c1.ptr);
    printf("c2.value=%d, *c2.ptr=%d\n", c2.value, *c2.ptr);
    printf("Note: value is independent, but ptr points to same location\n");
}

int main() {
    printf("STRUCTURE COMPREHENSIVE EXAMPLES\n");
    printf("================================\n");
    
    srand(time(NULL));
    
    demonstrateBasicStructure();
    demonstrateFunctionsWithStructures();
    demonstrateArrayOfStructures();
    demonstrateDynamicStructures();
    demonstrateNestedStructures();
    demonstrateLinkedList();
    demonstrateBitFields();
    demonstrateUnionInStructure();
    demonstrateStructurePadding();
    demonstrateComplexStructure();
    demonstrateStructureCopy();
    
    return 0;
}