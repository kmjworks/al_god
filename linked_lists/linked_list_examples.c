#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Singly linked list node
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Doubly linked list node
typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

// Function prototypes
Node* createNode(int data);
void displayList(Node* head);
void freeList(Node** head);

// Basic node creation
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

// Display linked list
void displayList(Node* head) {
    if (head == NULL) {
        printf("Empty list\n");
        return;
    }
    
    Node* current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" -> NULL\n");
}

// Free entire list
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

// Insertion operations demo
void insertion_demo() {
    printf("\n=== Insertion Operations Demo ===\n");
    
    Node* head = NULL;
    
    // Insert at beginning
    printf("Inserting at beginning: 30, 20, 10\n");
    for (int i = 3; i >= 1; i--) {
        Node* newNode = createNode(i * 10);
        newNode->next = head;
        head = newNode;
    }
    displayList(head);
    
    // Insert at end
    printf("\nInserting at end: 40, 50\n");
    Node* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = createNode(40);
    temp->next->next = createNode(50);
    displayList(head);
    
    // Insert at position
    printf("\nInserting 25 at position 2\n");
    temp = head;
    for (int i = 0; i < 1; i++) {
        temp = temp->next;
    }
    Node* newNode = createNode(25);
    newNode->next = temp->next;
    temp->next = newNode;
    displayList(head);
    
    freeList(&head);
}

// Deletion operations demo
void deletion_demo() {
    printf("\n=== Deletion Operations Demo ===\n");
    
    // Create list: 10 -> 20 -> 30 -> 40 -> 50
    Node* head = NULL;
    Node* tail = NULL;
    for (int i = 1; i <= 5; i++) {
        Node* newNode = createNode(i * 10);
        if (head == NULL) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    printf("Original list:\n");
    displayList(head);
    
    // Delete from beginning
    printf("\nDelete from beginning:\n");
    Node* temp = head;
    head = head->next;
    free(temp);
    displayList(head);
    
    // Delete from end
    printf("\nDelete from end:\n");
    temp = head;
    while (temp->next->next != NULL) {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
    displayList(head);
    
    // Delete by value (30)
    printf("\nDelete value 30:\n");
    temp = head;
    while (temp->next != NULL && temp->next->data != 30) {
        temp = temp->next;
    }
    if (temp->next != NULL) {
        Node* toDelete = temp->next;
        temp->next = temp->next->next;
        free(toDelete);
    }
    displayList(head);
    
    freeList(&head);
}

// Search operations
void search_demo() {
    printf("\n=== Search Operations Demo ===\n");
    
    // Create list
    Node* head = NULL;
    int values[] = {15, 23, 8, 23, 42, 23, 16};
    
    for (int i = 6; i >= 0; i--) {
        Node* newNode = createNode(values[i]);
        newNode->next = head;
        head = newNode;
    }
    
    printf("List: ");
    displayList(head);
    
    // Linear search
    int searchValue = 42;
    Node* current = head;
    int position = 0;
    bool found = false;
    
    while (current != NULL) {
        if (current->data == searchValue) {
            printf("Found %d at position %d\n", searchValue, position);
            found = true;
            break;
        }
        current = current->next;
        position++;
    }
    
    if (!found) {
        printf("%d not found in the list\n", searchValue);
    }
    
    // Count occurrences
    searchValue = 23;
    int count = 0;
    current = head;
    
    while (current != NULL) {
        if (current->data == searchValue) {
            count++;
        }
        current = current->next;
    }
    
    printf("Number of occurrences of %d: %d\n", searchValue, count);
    
    freeList(&head);
}

// Reverse list demonstration
void reverse_demo() {
    printf("\n=== Reverse List Demo ===\n");
    
    // Create list: 1 -> 2 -> 3 -> 4 -> 5
    Node* head = NULL;
    for (int i = 5; i >= 1; i--) {
        Node* newNode = createNode(i);
        newNode->next = head;
        head = newNode;
    }
    
    printf("Original list: ");
    displayList(head);
    
    // Reverse the list
    Node* prev = NULL;
    Node* current = head;
    Node* next = NULL;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
    
    printf("Reversed list: ");
    displayList(head);
    
    freeList(&head);
}

// Find middle element
void find_middle_demo() {
    printf("\n=== Find Middle Element Demo ===\n");
    
    // Test with odd number of elements
    Node* head1 = NULL;
    for (int i = 7; i >= 1; i--) {
        Node* newNode = createNode(i);
        newNode->next = head1;
        head1 = newNode;
    }
    
    printf("List with odd elements: ");
    displayList(head1);
    
    Node* slow = head1;
    Node* fast = head1;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    printf("Middle element: %d\n", slow->data);
    
    // Test with even number of elements
    Node* head2 = NULL;
    for (int i = 6; i >= 1; i--) {
        Node* newNode = createNode(i);
        newNode->next = head2;
        head2 = newNode;
    }
    
    printf("\nList with even elements: ");
    displayList(head2);
    
    slow = head2;
    fast = head2;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    printf("Middle element: %d\n", slow->data);
    
    freeList(&head1);
    freeList(&head2);
}

// Detect and handle cycles
void cycle_detection_demo() {
    printf("\n=== Cycle Detection Demo ===\n");
    
    // Create a list with cycle
    Node* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);
    
    // Create cycle: 5 -> 3
    head->next->next->next->next->next = head->next->next;
    
    printf("Created list with cycle at node 3\n");
    
    // Detect cycle using Floyd's algorithm
    Node* slow = head;
    Node* fast = head;
    bool hasCycle = false;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            hasCycle = true;
            break;
        }
    }
    
    if (hasCycle) {
        printf("Cycle detected!\n");
        
        // Find start of cycle
        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        
        printf("Cycle starts at node with value: %d\n", slow->data);
        
        // Break the cycle for cleanup
        Node* temp = head;
        while (temp->next != slow) {
            temp = temp->next;
        }
        while (temp->next != slow) {
            temp = temp->next;
        }
        temp->next = NULL;
    } else {
        printf("No cycle detected\n");
    }
    
    // Test with no cycle
    Node* head2 = NULL;
    for (int i = 5; i >= 1; i--) {
        Node* newNode = createNode(i);
        newNode->next = head2;
        head2 = newNode;
    }
    
    printf("\nTesting list without cycle: ");
    displayList(head2);
    
    slow = head2;
    fast = head2;
    hasCycle = false;
    
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        
        if (slow == fast) {
            hasCycle = true;
            break;
        }
    }
    
    printf("Cycle detected: %s\n", hasCycle ? "Yes" : "No");
    
    freeList(&head2);
    // Note: head1 cycle was broken, so normal free won't work properly
}

// Merge sorted lists
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

void merge_lists_demo() {
    printf("\n=== Merge Sorted Lists Demo ===\n");
    
    // Create first sorted list: 1 -> 3 -> 5 -> 7
    Node* list1 = createNode(1);
    list1->next = createNode(3);
    list1->next->next = createNode(5);
    list1->next->next->next = createNode(7);
    
    // Create second sorted list: 2 -> 4 -> 6 -> 8
    Node* list2 = createNode(2);
    list2->next = createNode(4);
    list2->next->next = createNode(6);
    list2->next->next->next = createNode(8);
    
    printf("List 1: ");
    displayList(list1);
    printf("List 2: ");
    displayList(list2);
    
    Node* merged = mergeSortedLists(list1, list2);
    
    printf("Merged list: ");
    displayList(merged);
    
    freeList(&merged);
}

// Remove duplicates
void remove_duplicates_demo() {
    printf("\n=== Remove Duplicates Demo ===\n");
    
    // Sorted list with duplicates
    Node* sorted = createNode(1);
    sorted->next = createNode(1);
    sorted->next->next = createNode(2);
    sorted->next->next->next = createNode(3);
    sorted->next->next->next->next = createNode(3);
    sorted->next->next->next->next->next = createNode(3);
    sorted->next->next->next->next->next->next = createNode(4);
    
    printf("Sorted list with duplicates: ");
    displayList(sorted);
    
    // Remove duplicates from sorted list
    Node* current = sorted;
    while (current->next != NULL) {
        if (current->data == current->next->data) {
            Node* temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
    
    printf("After removing duplicates: ");
    displayList(sorted);
    
    freeList(&sorted);
    
    // Unsorted list with duplicates
    Node* unsorted = createNode(4);
    unsorted->next = createNode(2);
    unsorted->next->next = createNode(3);
    unsorted->next->next->next = createNode(2);
    unsorted->next->next->next->next = createNode(1);
    unsorted->next->next->next->next->next = createNode(4);
    
    printf("\nUnsorted list with duplicates: ");
    displayList(unsorted);
    
    // Remove duplicates from unsorted list
    current = unsorted;
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
    
    printf("After removing duplicates: ");
    displayList(unsorted);
    
    freeList(&unsorted);
}

// Doubly linked list operations
DNode* createDNode(int data) {
    DNode* newNode = (DNode*)malloc(sizeof(DNode));
    if (newNode == NULL) return NULL;
    
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void displayDList(DNode* head) {
    if (head == NULL) {
        printf("Empty list\n");
        return;
    }
    
    printf("NULL <-> ");
    DNode* current = head;
    while (current != NULL) {
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void doubly_linked_demo() {
    printf("\n=== Doubly Linked List Demo ===\n");
    
    DNode* head = NULL;
    
    // Insert at beginning
    for (int i = 3; i >= 1; i--) {
        DNode* newNode = createDNode(i * 10);
        newNode->next = head;
        if (head != NULL) {
            head->prev = newNode;
        }
        head = newNode;
    }
    
    printf("After inserting at beginning: ");
    displayDList(head);
    
    // Insert at end
    DNode* tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    
    DNode* newNode = createDNode(40);
    tail->next = newNode;
    newNode->prev = tail;
    
    printf("After inserting 40 at end: ");
    displayDList(head);
    
    // Traverse backwards
    printf("Backward traversal: ");
    tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    
    while (tail != NULL) {
        printf("%d ", tail->data);
        tail = tail->prev;
    }
    printf("\n");
    
    // Delete a node (20)
    DNode* toDelete = head->next;
    toDelete->prev->next = toDelete->next;
    toDelete->next->prev = toDelete->prev;
    free(toDelete);
    
    printf("After deleting 20: ");
    displayDList(head);
    
    // Free doubly linked list
    while (head != NULL) {
        DNode* temp = head;
        head = head->next;
        free(temp);
    }
}

// Get Nth node from end
void nth_from_end_demo() {
    printf("\n=== Nth Node from End Demo ===\n");
    
    // Create list: 10 -> 20 -> 30 -> 40 -> 50
    Node* head = NULL;
    for (int i = 5; i >= 1; i--) {
        Node* newNode = createNode(i * 10);
        newNode->next = head;
        head = newNode;
    }
    
    printf("List: ");
    displayList(head);
    
    // Find different positions from end
    for (int n = 1; n <= 6; n++) {
        Node* fast = head;
        Node* slow = head;
        
        // Move fast n nodes ahead
        bool valid = true;
        for (int i = 0; i < n; i++) {
            if (fast == NULL) {
                valid = false;
                break;
            }
            fast = fast->next;
        }
        
        if (valid) {
            // Move both until fast reaches end
            while (fast != NULL) {
                slow = slow->next;
                fast = fast->next;
            }
            printf("%dth node from end: %d\n", n, slow->data);
        } else {
            printf("%dth node from end: Not found (list too short)\n", n);
        }
    }
    
    freeList(&head);
}

// Palindrome check
bool isPalindrome(Node* head) {
    if (head == NULL || head->next == NULL) return true;
    
    // Find middle
    Node* slow = head;
    Node* fast = head;
    Node* prev_slow = head;
    
    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;
        prev_slow = slow;
        slow = slow->next;
    }
    
    // Reverse second half
    Node* second_half = slow;
    prev_slow->next = NULL;
    
    Node* prev = NULL;
    Node* current = second_half;
    Node* next;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    second_half = prev;
    
    // Compare halves
    Node* first_half = head;
    bool is_palindrome = true;
    
    while (second_half != NULL) {
        if (first_half->data != second_half->data) {
            is_palindrome = false;
            break;
        }
        first_half = first_half->next;
        second_half = second_half->next;
    }
    
    return is_palindrome;
}

void palindrome_demo() {
    printf("\n=== Palindrome Check Demo ===\n");
    
    // Test case 1: Palindrome (odd length)
    Node* list1 = createNode(1);
    list1->next = createNode(2);
    list1->next->next = createNode(3);
    list1->next->next->next = createNode(2);
    list1->next->next->next->next = createNode(1);
    
    printf("List 1: ");
    displayList(list1);
    printf("Is palindrome: %s\n", isPalindrome(list1) ? "Yes" : "No");
    
    // Test case 2: Palindrome (even length)
    Node* list2 = createNode(1);
    list2->next = createNode(2);
    list2->next->next = createNode(2);
    list2->next->next->next = createNode(1);
    
    printf("\nList 2: ");
    displayList(list2);
    printf("Is palindrome: %s\n", isPalindrome(list2) ? "Yes" : "No");
    
    // Test case 3: Not palindrome
    Node* list3 = createNode(1);
    list3->next = createNode(2);
    list3->next->next = createNode(3);
    list3->next->next->next = createNode(4);
    
    printf("\nList 3: ");
    displayList(list3);
    printf("Is palindrome: %s\n", isPalindrome(list3) ? "Yes" : "No");
    
    // Note: Lists are modified by palindrome check, so careful with cleanup
}

int main() {
    printf("LINKED LIST COMPREHENSIVE EXAMPLES\n");
    printf("==================================\n");
    
    insertion_demo();
    deletion_demo();
    search_demo();
    reverse_demo();
    find_middle_demo();
    cycle_detection_demo();
    merge_lists_demo();
    remove_duplicates_demo();
    doubly_linked_demo();
    nth_from_end_demo();
    palindrome_demo();
    
    printf("\nAll demonstrations completed!\n");
    
    return 0;
}