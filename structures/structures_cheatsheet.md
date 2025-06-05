# Structures Cheat Sheet - C Programming

## What are Structures?
Structures (structs) are user-defined data types that group related data elements of different types under a single name. They allow you to create complex data types that represent real-world entities.

## Basic Declaration and Usage

### Simple Structure
```c
struct Point {
    int x;
    int y;
};

// Creating structure variables
struct Point p1;
struct Point p2 = {10, 20};  // Initialization
```

### Typedef with Structures
```c
// Method 1: Separate typedef
struct Student {
    char name[50];
    int id;
    float gpa;
};
typedef struct Student Student;

// Method 2: Combined typedef
typedef struct {
    char name[50];
    int age;
    char email[100];
} Person;

// Method 3: Named struct with typedef
typedef struct Node {
    int data;
    struct Node *next;  // Self-referential
} Node;
```

## Accessing Members

### Dot Operator (.)
```c
struct Rectangle {
    int width;
    int height;
};

struct Rectangle rect;
rect.width = 10;
rect.height = 20;
int area = rect.width * rect.height;
```

### Arrow Operator (->)
```c
struct Rectangle *ptr = &rect;
ptr->width = 15;
ptr->height = 25;
int area = ptr->width * ptr->height;

// Equivalent to:
int area2 = (*ptr).width * (*ptr).height;
```

## Initialization Methods

### 1. Designated Initializers (C99)
```c
struct Person {
    char name[50];
    int age;
    float salary;
};

struct Person p1 = {.age = 25, .name = "John", .salary = 50000.0};
struct Person p2 = {.name = "Alice"};  // Other members set to 0
```

### 2. Compound Literals
```c
struct Point {
    int x, y;
};

// Assign using compound literal
struct Point p;
p = (struct Point){10, 20};

// Pass to function
void printPoint(struct Point p);
printPoint((struct Point){5, 15});
```

## Nested Structures

### Structure within Structure
```c
struct Date {
    int day;
    int month;
    int year;
};

struct Employee {
    char name[50];
    int id;
    struct Date joinDate;
    struct Date birthDate;
};

struct Employee emp = {
    "John Doe",
    12345,
    {15, 6, 2020},    // joinDate
    {10, 3, 1990}     // birthDate
};

// Accessing nested members
printf("Join year: %d\n", emp.joinDate.year);
```

## Arrays and Structures

### Array of Structures
```c
struct Student {
    char name[50];
    int roll;
    float marks;
};

struct Student class[100];

// Initialize
class[0] = (struct Student){"Alice", 1, 95.5};
class[1].roll = 2;
strcpy(class[1].name, "Bob");
class[1].marks = 87.0;

// Access
for (int i = 0; i < 2; i++) {
    printf("%s: %.1f\n", class[i].name, class[i].marks);
}
```

### Structure with Array Members
```c
struct Matrix {
    int rows;
    int cols;
    int data[10][10];
};

struct Matrix m = {
    .rows = 2,
    .cols = 3,
    .data = {{1, 2, 3}, {4, 5, 6}}
};
```

## Dynamic Structures

### Dynamic Allocation
```c
typedef struct {
    char *name;
    int *scores;
    int numScores;
} Student;

// Allocate structure
Student *s = (Student*)malloc(sizeof(Student));

// Allocate members
s->name = (char*)malloc(50 * sizeof(char));
s->numScores = 5;
s->scores = (int*)malloc(s->numScores * sizeof(int));

// Use
strcpy(s->name, "John");
for (int i = 0; i < s->numScores; i++) {
    s->scores[i] = 80 + i;
}

// Free in reverse order
free(s->scores);
free(s->name);
free(s);
```

### Dynamic Array of Structures
```c
typedef struct {
    int x, y;
} Point;

int n = 10;
Point *points = (Point*)malloc(n * sizeof(Point));

for (int i = 0; i < n; i++) {
    points[i].x = i;
    points[i].y = i * i;
}

free(points);
```

## Function and Structures

### Pass by Value
```c
struct Rectangle {
    int width, height;
};

int getArea(struct Rectangle r) {
    return r.width * r.height;  // Copy of structure
}
```

### Pass by Reference
```c
void doubleSize(struct Rectangle *r) {
    r->width *= 2;
    r->height *= 2;  // Original modified
}

struct Rectangle rect = {10, 20};
doubleSize(&rect);
```

### Return Structure
```c
struct Point createPoint(int x, int y) {
    struct Point p = {x, y};
    return p;  // Return by value
}

struct Point* createDynamicPoint(int x, int y) {
    struct Point *p = malloc(sizeof(struct Point));
    p->x = x;
    p->y = y;
    return p;  // Return pointer
}
```

## Bit Fields
```c
struct Flags {
    unsigned int isActive : 1;    // 1 bit
    unsigned int priority : 3;    // 3 bits (0-7)
    unsigned int category : 4;    // 4 bits (0-15)
};

struct Flags f = {1, 5, 10};
printf("Size: %zu bytes\n", sizeof(struct Flags));  // Often 4 bytes
```

## Unions in Structures
```c
typedef struct {
    char type;  // 'i' for int, 'f' for float, 's' for string
    union {
        int intVal;
        float floatVal;
        char strVal[20];
    } value;
} Variable;

Variable v1 = {'i', .value.intVal = 42};
Variable v2 = {'f', .value.floatVal = 3.14};
```

## Common Patterns

### 1. Linked List Node
```c
typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* createNode(int data) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
```

### 2. Binary Tree Node
```c
typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;
```

### 3. Complex Number
```c
typedef struct {
    double real;
    double imag;
} Complex;

Complex addComplex(Complex a, Complex b) {
    Complex result = {
        .real = a.real + b.real,
        .imag = a.imag + b.imag
    };
    return result;
}
```

### 4. File Record
```c
typedef struct {
    char filename[256];
    size_t size;
    time_t modified;
    struct {
        unsigned int read : 1;
        unsigned int write : 1;
        unsigned int execute : 1;
    } permissions;
} FileInfo;
```

## Structure Padding and Alignment
```c
// Inefficient layout
struct BadLayout {
    char a;    // 1 byte + 7 padding
    double b;  // 8 bytes
    char c;    // 1 byte + 7 padding
};  // Total: 24 bytes

// Efficient layout
struct GoodLayout {
    double b;  // 8 bytes
    char a;    // 1 byte
    char c;    // 1 byte + 6 padding
};  // Total: 16 bytes

// Pack structure (compiler-specific)
#pragma pack(1)
struct PackedStruct {
    char a;
    double b;
    char c;
};  // Total: 10 bytes
#pragma pack()
```

## Structure Comparison
```c
typedef struct {
    int x, y;
} Point;

// Cannot use == operator directly
Point p1 = {10, 20};
Point p2 = {10, 20};

// Method 1: Member-wise comparison
int areEqual = (p1.x == p2.x && p1.y == p2.y);

// Method 2: memcmp (careful with padding!)
int areEqual2 = (memcmp(&p1, &p2, sizeof(Point)) == 0);

// Method 3: Custom function
int pointsEqual(Point a, Point b) {
    return a.x == b.x && a.y == b.y;
}
```

## Flexible Array Member (C99)
```c
typedef struct {
    int length;
    char data[];  // Flexible array member
} String;

// Allocate
String *str = malloc(sizeof(String) + 20);
str->length = 20;
strcpy(str->data, "Hello, World!");
```

## Anonymous Structures (C11)
```c
struct Vector3D {
    union {
        struct {
            float x, y, z;
        };
        float components[3];
    };
};

struct Vector3D v = {1.0, 2.0, 3.0};
printf("x=%f, components[0]=%f\n", v.x, v.components[0]);
```

## Quick Reference
| Operation | Syntax | Example |
|-----------|--------|---------|
| Declare struct | `struct Name { members };` | `struct Point { int x, y; };` |
| Create variable | `struct Name var;` | `struct Point p;` |
| With typedef | `typedef struct { } Name;` | `typedef struct { } Point;` |
| Access member | `var.member` | `p.x = 10;` |
| Pointer access | `ptr->member` | `ptr->x = 10;` |
| Initialize | `{ values }` | `{10, 20}` |
| Designated init | `{.member = value}` | `{.x = 10}` |
| Sizeof | `sizeof(struct Name)` | `sizeof(struct Point)` |

## Best Practices
1. Use typedef to avoid writing 'struct' repeatedly
2. Initialize structures when declaring them
3. Use designated initializers for clarity
4. Be aware of structure padding and alignment
5. Pass large structures by pointer to functions
6. Free dynamically allocated members before the structure
7. Use const for read-only structure parameters

## Common Pitfalls
1. **Assignment vs Comparison**: Can assign with =, but not compare with ==
2. **Padding**: Structure size may be larger than sum of members
3. **Self-referential**: Must use struct tag, not typedef name
4. **Dynamic members**: Remember to allocate and free separately
5. **Shallow copy**: Assignment copies pointers, not pointed data