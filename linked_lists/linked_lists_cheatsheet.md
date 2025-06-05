# Linked Lists Cheat Sheet - C Programming

## What is a Linked List?
A linked list is a linear data structure where elements are stored in nodes, and each node contains data and a pointer to the next node. Unlike arrays, linked lists don't require contiguous memory.

## Types of Linked Lists

### 1. Singly Linked List
```c
typedef struct Node {
    int data;
    struct Node* next;
} Node;
```

### 2. Doubly Linked List
```c
typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;
```

### 3. Circular Linked List
- Last node points back to first node
- Can be singly or doubly linked

## Basic Operations - Singly Linked List

### Node Creation
```c
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
```

### Insertion Operations

#### 1. Insert at Beginning
```c
void insertAtBeginning(Node** head, int data) {
    Node* newNode = createNode(data);
    if (newNode == NULL) return;
    
    newNode->next = *head;
    *head = newNode;
}
```

#### 2. Insert at End
```c
void insertAtEnd(Node** head, int data) {
    Node* newNode = createNode(data);
    if (newNode == NULL) return;
    
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}
```

#### 3. Insert at Position
```c
void insertAtPosition(Node** head, int data, int position) {
    if (position < 0) {
        printf("Invalid position\n");
        return;
    }
    
    if (position == 0) {
        insertAtBeginning(head, data);
        return;
    }
    
    Node* newNode = createNode(data);
    if (newNode == NULL) return;
    
    Node* temp = *head;
    for (int i = 0; i < position - 1 && temp != NULL; i++) {
        temp = temp->next;
    }
    
    if (temp == NULL) {
        printf("Position out of bounds\n");
        free(newNode);
        return;
    }
    
    newNode->next = temp->next;
    temp->next = newNode;
}
```

#### 4. Insert in Sorted Order
```c
void insertSorted(Node** head, int data) {
    Node* newNode = createNode(data);
    if (newNode == NULL) return;
    
    if (*head == NULL || (*head)->data >= data) {
        newNode->next = *head;
        *head = newNode;
        return;
    }
    
    Node* current = *head;
    while (current->next != NULL && current->next->data < data) {
        current = current->next;
    }
    
    newNode->next = current->next;
    current->next = newNode;
}
```

### Deletion Operations

#### 1. Delete from Beginning
```c
void deleteFromBeginning(Node** head) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    Node* temp = *head;
    *head = (*head)->next;
    free(temp);
}
```

#### 2. Delete from End
```c
void deleteFromEnd(Node** head) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }
    
    Node* temp = *head;
    while (temp->next->next != NULL) {
        temp = temp->next;
    }
    
    free(temp->next);
    temp->next = NULL;
}
```

#### 3. Delete by Value
```c
void deleteByValue(Node** head, int value) {
    if (*head == NULL) return;
    
    if ((*head)->data == value) {
        Node* temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }
    
    Node* current = *head;
    while (current->next != NULL && current->next->data != value) {
        current = current->next;
    }
    
    if (current->next != NULL) {
        Node* temp = current->next;
        current->next = current->next->next;
        free(temp);
    }
}
```

#### 4. Delete at Position
```c
void deleteAtPosition(Node** head, int position) {
    if (*head == NULL || position < 0) return;
    
    if (position == 0) {
        deleteFromBeginning(head);
        return;
    }
    
    Node* temp = *head;
    for (int i = 0; i < position - 1 && temp->next != NULL; i++) {
        temp = temp->next;
    }
    
    if (temp->next == NULL) {
        printf("Position out of bounds\n");
        return;
    }
    
    Node* nodeToDelete = temp->next;
    temp->next = temp->next->next;
    free(nodeToDelete);
}
```

### Search Operations

#### Linear Search
```c
Node* search(Node* head, int key) {
    Node* current = head;
    int position = 0;
    
    while (current != NULL) {
        if (current->data == key) {
            printf("Found at position %d\n", position);
            return current;
        }
        current = current->next;
        position++;
    }
    
    printf("Element not found\n");
    return NULL;
}
```

#### Count Occurrences
```c
int countOccurrences(Node* head, int key) {
    int count = 0;
    Node* current = head;
    
    while (current != NULL) {
        if (current->data == key) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}
```

### List Operations

#### 1. Get Length
```c
int getLength(Node* head) {
    int count = 0;
    Node* current = head;
    
    while (current != NULL) {
        count++;
        current = current->next;
    }
    
    return count;
}

// Recursive version
int getLengthRecursive(Node* head) {
    if (head == NULL) return 0;
    return 1 + getLengthRecursive(head->next);
}
```

#### 2. Display List
```c
void displayList(Node* head) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}
```

#### 3. Reverse List
```c
// Iterative approach
void reverseList(Node** head) {
    Node* prev = NULL;
    Node* current = *head;
    Node* next = NULL;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    *head = prev;
}

// Recursive approach
Node* reverseListRecursive(Node* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    
    Node* rest = reverseListRecursive(head->next);
    head->next->next = head;
    head->next = NULL;
    
    return rest;
}
```

#### 4. Find Middle Element
```c
Node* findMiddle(Node* head) {
    if (head == NULL) return NULL;
    
    Node* slow = head;
    Node* fast = head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}
```

#### 5. Detect Cycle
```c
bool hasCycle(Node* head) {
    if (head == NULL) return false;
    
    Node* slow = head;
    Node* fast = head;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            return true;
        }
    }
    
    return false;
}

// Find start of cycle
Node* findCycleStart(Node* head) {
    Node* slow = head;
    Node* fast = head;
    
    // Find if cycle exists
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            // Cycle detected
            slow = head;
            while (slow != fast) {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }
    
    return NULL;
}
```

#### 6. Remove Duplicates
```c
// From sorted list
void removeDuplicatesSorted(Node* head) {
    if (head == NULL) return;
    
    Node* current = head;
    
    while (current->next != NULL) {
        if (current->data == current->next->data) {
            Node* temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
}

// From unsorted list
void removeDuplicatesUnsorted(Node* head) {
    if (head == NULL) return;
    
    Node* current = head;
    
    while (current != NULL && current->next != NULL) {
        Node* runner = current;
        
        while (runner->next != NULL) {
            if (current->data == runner->next->data) {
                Node* temp = runner->next;
                runner->next = runner->next->next;
                free(temp);
            } else {
                runner = runner->next;
            }
        }
        current = current->next;
    }
}
```

## Doubly Linked List Operations

### Node Structure and Creation
```c
typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

DNode* createDNode(int data) {
    DNode* newNode = (DNode*)malloc(sizeof(DNode));
    if (newNode == NULL) return NULL;
    
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}
```

### Insert at Beginning (Doubly)
```c
void insertAtBeginningDLL(DNode** head, int data) {
    DNode* newNode = createDNode(data);
    if (newNode == NULL) return;
    
    newNode->next = *head;
    
    if (*head != NULL) {
        (*head)->prev = newNode;
    }
    
    *head = newNode;
}
```

### Insert at End (Doubly)
```c
void insertAtEndDLL(DNode** head, int data) {
    DNode* newNode = createDNode(data);
    if (newNode == NULL) return;
    
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    
    DNode* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    
    temp->next = newNode;
    newNode->prev = temp;
}
```

### Delete Node (Doubly)
```c
void deleteNodeDLL(DNode** head, DNode* nodeToDelete) {
    if (*head == NULL || nodeToDelete == NULL) return;
    
    if (*head == nodeToDelete) {
        *head = nodeToDelete->next;
    }
    
    if (nodeToDelete->next != NULL) {
        nodeToDelete->next->prev = nodeToDelete->prev;
    }
    
    if (nodeToDelete->prev != NULL) {
        nodeToDelete->prev->next = nodeToDelete->next;
    }
    
    free(nodeToDelete);
}
```

## Circular Linked List

### Check if Circular
```c
bool isCircular(Node* head) {
    if (head == NULL) return true;
    
    Node* temp = head->next;
    
    while (temp != NULL && temp != head) {
        temp = temp->next;
    }
    
    return (temp == head);
}
```

### Insert in Circular List
```c
void insertInCircular(Node** head, int data) {
    Node* newNode = createNode(data);
    if (newNode == NULL) return;
    
    if (*head == NULL) {
        *head = newNode;
        newNode->next = newNode;
        return;
    }
    
    Node* temp = *head;
    while (temp->next != *head) {
        temp = temp->next;
    }
    
    newNode->next = *head;
    temp->next = newNode;
}
```

## Advanced Operations

### 1. Merge Two Sorted Lists
```c
Node* mergeSortedLists(Node* a, Node* b) {
    if (a == NULL) return b;
    if (b == NULL) return a;
    
    if (a->data <= b->data) {
        a->next = mergeSortedLists(a->next, b);
        return a;
    } else {
        b->next = mergeSortedLists(a, b->next);
        return b;
    }
}
```

### 2. Split List into Two Halves
```c
void splitList(Node* head, Node** firstHalf, Node** secondHalf) {
    if (head == NULL || head->next == NULL) {
        *firstHalf = head;
        *secondHalf = NULL;
        return;
    }
    
    Node* slow = head;
    Node* fast = head->next;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    *firstHalf = head;
    *secondHalf = slow->next;
    slow->next = NULL;
}
```

### 3. Intersection of Two Lists
```c
Node* findIntersection(Node* head1, Node* head2) {
    int len1 = getLength(head1);
    int len2 = getLength(head2);
    
    int diff = abs(len1 - len2);
    
    Node* ptr1 = (len1 > len2) ? head1 : head2;
    Node* ptr2 = (len1 > len2) ? head2 : head1;
    
    // Advance the longer list
    for (int i = 0; i < diff; i++) {
        ptr1 = ptr1->next;
    }
    
    // Find intersection
    while (ptr1 != NULL && ptr2 != NULL) {
        if (ptr1 == ptr2) {
            return ptr1;
        }
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
    
    return NULL;
}
```

### 4. Nth Node from End
```c
Node* getNthFromEnd(Node* head, int n) {
    if (head == NULL || n <= 0) return NULL;
    
    Node* fast = head;
    Node* slow = head;
    
    // Move fast pointer n nodes ahead
    for (int i = 0; i < n; i++) {
        if (fast == NULL) return NULL;
        fast = fast->next;
    }
    
    // Move both pointers until fast reaches end
    while (fast != NULL) {
        slow = slow->next;
        fast = fast->next;
    }
    
    return slow;
}
```

### 5. Palindrome Check
```c
bool isPalindrome(Node* head) {
    if (head == NULL || head->next == NULL) return true;
    
    // Find middle
    Node* slow = head;
    Node* fast = head;
    Node* prev_slow = head;
    Node* midnode = NULL;
    
    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;
        prev_slow = slow;
        slow = slow->next;
    }
    
    // Handle odd length
    if (fast != NULL) {
        midnode = slow;
        slow = slow->next;
    }
    
    // Reverse second half
    Node* second_half = slow;
    prev_slow->next = NULL;
    reverseList(&second_half);
    
    // Compare
    bool result = true;
    Node* ptr1 = head;
    Node* ptr2 = second_half;
    
    while (ptr2 != NULL) {
        if (ptr1->data != ptr2->data) {
            result = false;
            break;
        }
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }
    
    // Restore list
    reverseList(&second_half);
    
    if (midnode != NULL) {
        prev_slow->next = midnode;
        midnode->next = second_half;
    } else {
        prev_slow->next = second_half;
    }
    
    return result;
}
```

## Memory Management

### Free Entire List
```c
void freeList(Node** head) {
    Node* current = *head;
    Node* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    
    *head = NULL;
}
```

## Common Mistakes and Solutions

### 1. Memory Leaks
```c
// BAD: Memory leak when deleting
void badDelete(Node** head) {
    *head = (*head)->next;  // Lost reference to first node!
}

// GOOD: Free memory before losing reference
void goodDelete(Node** head) {
    Node* temp = *head;
    *head = (*head)->next;
    free(temp);
}
```

### 2. Null Pointer Access
```c
// BAD: No NULL check
void badAccess(Node* head) {
    printf("%d\n", head->data);  // Crash if head is NULL
}

// GOOD: Check for NULL
void goodAccess(Node* head) {
    if (head != NULL) {
        printf("%d\n", head->data);
    }
}
```

### 3. Lost Head Pointer
```c
// BAD: Modifying head directly
void badTraverse(Node* head) {
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;  // Lost original head!
    }
}

// GOOD: Use temporary pointer
void goodTraverse(Node* head) {
    Node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
}
```

## Time Complexity

| Operation | Array | Linked List |
|-----------|-------|-------------|
| Access by index | O(1) | O(n) |
| Insert at beginning | O(n) | O(1) |
| Insert at end | O(1)* | O(n) |
| Insert at position | O(n) | O(n) |
| Delete at beginning | O(n) | O(1) |
| Delete at end | O(1) | O(n) |
| Search | O(n) | O(n) |

*Array insert at end is O(1) if not full, O(n) if reallocation needed

## When to Use Linked Lists

**Use Linked Lists when:**
- Frequent insertion/deletion at beginning
- Size varies significantly
- No random access needed
- Implementing stacks, queues

**Use Arrays when:**
- Need random access
- Size is relatively fixed
- Cache performance is important
- Need to store small data types

## Quick Reference

| Function | Purpose | Time |
|----------|---------|------|
| `insertAtBeginning()` | Add node at start | O(1) |
| `insertAtEnd()` | Add node at end | O(n) |
| `deleteByValue()` | Remove first occurrence | O(n) |
| `search()` | Find element | O(n) |
| `reverseList()` | Reverse entire list | O(n) |
| `findMiddle()` | Get middle element | O(n) |
| `hasCycle()` | Detect loop | O(n) |
| `getLength()` | Count nodes | O(n) |

## Exam Tips
1. Always check for NULL pointers
2. Don't lose the head pointer reference
3. Free memory when deleting nodes
4. Use double pointers for modifying head
5. Draw diagrams for pointer manipulation
6. Consider edge cases (empty list, single node)
7. Remember prev/next updates for doubly linked lists
8. Use fast/slow pointer technique for various problems