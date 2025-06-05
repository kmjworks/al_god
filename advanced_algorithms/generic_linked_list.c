#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Generic linked list node
typedef struct ListNode {
    void *data;
    struct ListNode *next;
    struct ListNode *prev;  // For doubly linked list
} ListNode;

// Generic linked list structure
typedef struct {
    ListNode *head;
    ListNode *tail;
    size_t size;
    size_t data_size;
    int (*compare)(const void *, const void *);
    void (*print)(const void *);
    void (*free_data)(void *);
} LinkedList;

// Create new node
ListNode* create_list_node(void *data, size_t data_size) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (!node) return NULL;
    
    node->data = malloc(data_size);
    if (!node->data) {
        free(node);
        return NULL;
    }
    
    memcpy(node->data, data, data_size);
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// Initialize linked list
LinkedList* create_linked_list(size_t data_size,
                              int (*compare)(const void *, const void *),
                              void (*print)(const void *),
                              void (*free_data)(void *)) {
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    if (!list) return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->data_size = data_size;
    list->compare = compare;
    list->print = print;
    list->free_data = free_data;
    return list;
}

// Insert at beginning
void list_push_front(LinkedList *list, void *data) {
    ListNode *node = create_list_node(data, list->data_size);
    if (!node) return;
    
    node->next = list->head;
    if (list->head) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }
    list->head = node;
    list->size++;
}

// Insert at end
void list_push_back(LinkedList *list, void *data) {
    ListNode *node = create_list_node(data, list->data_size);
    if (!node) return;
    
    node->prev = list->tail;
    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
}

// Insert at position
void list_insert_at(LinkedList *list, void *data, size_t position) {
    if (position == 0) {
        list_push_front(list, data);
        return;
    }
    if (position >= list->size) {
        list_push_back(list, data);
        return;
    }
    
    ListNode *node = create_list_node(data, list->data_size);
    if (!node) return;
    
    ListNode *current = list->head;
    for (size_t i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    node->next = current->next;
    node->prev = current;
    if (current->next) {
        current->next->prev = node;
    }
    current->next = node;
    list->size++;
}

// Insert sorted (maintains sorted order)
void list_insert_sorted(LinkedList *list, void *data) {
    if (!list->head || list->compare(data, list->head->data) < 0) {
        list_push_front(list, data);
        return;
    }
    
    ListNode *current = list->head;
    while (current->next && list->compare(data, current->next->data) >= 0) {
        current = current->next;
    }
    
    ListNode *node = create_list_node(data, list->data_size);
    if (!node) return;
    
    node->next = current->next;
    node->prev = current;
    if (current->next) {
        current->next->prev = node;
    } else {
        list->tail = node;
    }
    current->next = node;
    list->size++;
}

// Remove from beginning
void* list_pop_front(LinkedList *list) {
    if (!list->head) return NULL;
    
    ListNode *node = list->head;
    void *data = malloc(list->data_size);
    if (!data) return NULL;
    
    memcpy(data, node->data, list->data_size);
    
    list->head = node->next;
    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }
    
    if (list->free_data) list->free_data(node->data);
    else free(node->data);
    free(node);
    list->size--;
    
    return data;
}

// Remove from end
void* list_pop_back(LinkedList *list) {
    if (!list->tail) return NULL;
    
    ListNode *node = list->tail;
    void *data = malloc(list->data_size);
    if (!data) return NULL;
    
    memcpy(data, node->data, list->data_size);
    
    list->tail = node->prev;
    if (list->tail) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }
    
    if (list->free_data) list->free_data(node->data);
    else free(node->data);
    free(node);
    list->size--;
    
    return data;
}

// Remove specific element
bool list_remove(LinkedList *list, void *data) {
    ListNode *current = list->head;
    
    while (current) {
        if (list->compare(current->data, data) == 0) {
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                list->head = current->next;
            }
            
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                list->tail = current->prev;
            }
            
            if (list->free_data) list->free_data(current->data);
            else free(current->data);
            free(current);
            list->size--;
            return true;
        }
        current = current->next;
    }
    return false;
}

// Find element
void* list_find(LinkedList *list, void *data) {
    ListNode *current = list->head;
    
    while (current) {
        if (list->compare(current->data, data) == 0) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

// Get element at index
void* list_get_at(LinkedList *list, size_t index) {
    if (index >= list->size) return NULL;
    
    ListNode *current;
    
    // Optimize by starting from tail if index is closer to end
    if (index > list->size / 2) {
        current = list->tail;
        for (size_t i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    } else {
        current = list->head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    }
    
    return current->data;
}

// Reverse the list
void list_reverse(LinkedList *list) {
    ListNode *current = list->head;
    ListNode *temp = NULL;
    
    list->tail = list->head;
    
    while (current) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }
    
    if (temp) {
        list->head = temp->prev;
    }
}

// Sort the list (using merge sort)
ListNode* merge_sorted_lists(ListNode *a, ListNode *b,
                            int (*compare)(const void *, const void *)) {
    if (!a) return b;
    if (!b) return a;
    
    if (compare(a->data, b->data) <= 0) {
        a->next = merge_sorted_lists(a->next, b, compare);
        if (a->next) a->next->prev = a;
        a->prev = NULL;
        return a;
    } else {
        b->next = merge_sorted_lists(a, b->next, compare);
        if (b->next) b->next->prev = b;
        b->prev = NULL;
        return b;
    }
}

ListNode* get_middle(ListNode *head) {
    if (!head) return head;
    
    ListNode *slow = head;
    ListNode *fast = head->next;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

ListNode* merge_sort_list(ListNode *head,
                         int (*compare)(const void *, const void *)) {
    if (!head || !head->next) return head;
    
    ListNode *middle = get_middle(head);
    ListNode *next_of_middle = middle->next;
    
    middle->next = NULL;
    if (next_of_middle) next_of_middle->prev = NULL;
    
    ListNode *left = merge_sort_list(head, compare);
    ListNode *right = merge_sort_list(next_of_middle, compare);
    
    return merge_sorted_lists(left, right, compare);
}

void list_sort(LinkedList *list) {
    list->head = merge_sort_list(list->head, list->compare);
    
    // Update tail pointer
    ListNode *current = list->head;
    while (current && current->next) {
        current = current->next;
    }
    list->tail = current;
}

// Print list
void list_print(LinkedList *list) {
    ListNode *current = list->head;
    printf("[");
    while (current) {
        list->print(current->data);
        if (current->next) printf(", ");
        current = current->next;
    }
    printf("]\n");
}

// Print list backwards
void list_print_reverse(LinkedList *list) {
    ListNode *current = list->tail;
    printf("[");
    while (current) {
        list->print(current->data);
        if (current->prev) printf(", ");
        current = current->prev;
    }
    printf("]\n");
}

// Apply function to each element
void list_for_each(LinkedList *list, void (*func)(void *)) {
    ListNode *current = list->head;
    while (current) {
        func(current->data);
        current = current->next;
    }
}

// Map function (create new list with transformed elements)
LinkedList* list_map(LinkedList *list, void *(*transform)(void *),
                    size_t new_data_size,
                    int (*new_compare)(const void *, const void *),
                    void (*new_print)(const void *),
                    void (*new_free)(void *)) {
    LinkedList *new_list = create_linked_list(new_data_size, new_compare,
                                             new_print, new_free);
    if (!new_list) return NULL;
    
    ListNode *current = list->head;
    while (current) {
        void *transformed = transform(current->data);
        list_push_back(new_list, transformed);
        if (new_free) new_free(transformed);
        else free(transformed);
        current = current->next;
    }
    
    return new_list;
}

// Filter function (create new list with elements that pass test)
LinkedList* list_filter(LinkedList *list, bool (*predicate)(void *)) {
    LinkedList *new_list = create_linked_list(list->data_size, list->compare,
                                             list->print, list->free_data);
    if (!new_list) return NULL;
    
    ListNode *current = list->head;
    while (current) {
        if (predicate(current->data)) {
            list_push_back(new_list, current->data);
        }
        current = current->next;
    }
    
    return new_list;
}

// Free entire list
void free_linked_list(LinkedList *list) {
    ListNode *current = list->head;
    while (current) {
        ListNode *next = current->next;
        if (list->free_data) list->free_data(current->data);
        else free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

// Comparison and utility functions
int compare_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void print_int(const void *data) {
    printf("%d", *(int *)data);
}

// Advanced example functions
void double_int(void *data) {
    *(int *)data *= 2;
}

void* square_int(void *data) {
    int *result = malloc(sizeof(int));
    *result = (*(int *)data) * (*(int *)data);
    return result;
}

bool is_even(void *data) {
    return *(int *)data % 2 == 0;
}

// Complex type example
typedef struct {
    int id;
    char name[50];
    double value;
} Item;

int compare_item_by_id(const void *a, const void *b) {
    return ((Item *)a)->id - ((Item *)b)->id;
}

void print_item(const void *data) {
    Item *item = (Item *)data;
    printf("{id:%d, name:%s, value:%.2f}", item->id, item->name, item->value);
}

// Main demonstration
int main() {
    printf("=== Generic Linked List Operations ===\n\n");
    
    // Basic operations
    LinkedList *list = create_linked_list(sizeof(int), compare_int, print_int, NULL);
    
    printf("1. Push operations:\n");
    int vals[] = {5, 3, 8, 1, 9};
    for (int i = 0; i < 5; i++) {
        list_push_back(list, &vals[i]);
    }
    printf("After push_back: ");
    list_print(list);
    
    int front_val = 0;
    list_push_front(list, &front_val);
    printf("After push_front(0): ");
    list_print(list);
    
    int insert_val = 7;
    list_insert_at(list, &insert_val, 3);
    printf("After insert_at(7, pos=3): ");
    list_print(list);
    
    printf("\n2. Pop operations:\n");
    int *popped = (int *)list_pop_front(list);
    printf("Popped from front: %d\n", *popped);
    free(popped);
    printf("List after pop_front: ");
    list_print(list);
    
    popped = (int *)list_pop_back(list);
    printf("Popped from back: %d\n", *popped);
    free(popped);
    printf("List after pop_back: ");
    list_print(list);
    
    printf("\n3. Reverse operation:\n");
    list_reverse(list);
    printf("Reversed list: ");
    list_print(list);
    printf("Print reverse: ");
    list_print_reverse(list);
    
    printf("\n4. Sort operation:\n");
    list_sort(list);
    printf("Sorted list: ");
    list_print(list);
    
    printf("\n5. Insert sorted:\n");
    int sorted_vals[] = {4, 6, 2};
    for (int i = 0; i < 3; i++) {
        list_insert_sorted(list, &sorted_vals[i]);
        printf("After inserting %d: ", sorted_vals[i]);
        list_print(list);
    }
    
    printf("\n6. Find and remove operations:\n");
    int find_val = 6;
    int *found = (int *)list_find(list, &find_val);
    printf("Find 6: %s\n", found ? "Found" : "Not found");
    
    list_remove(list, &find_val);
    printf("After removing 6: ");
    list_print(list);
    
    printf("\n7. Functional operations:\n");
    printf("Original: ");
    list_print(list);
    
    list_for_each(list, double_int);
    printf("After doubling each element: ");
    list_print(list);
    
    LinkedList *squared = list_map(list, square_int, sizeof(int), 
                                  compare_int, print_int, NULL);
    printf("Squared values: ");
    list_print(squared);
    
    LinkedList *evens = list_filter(list, is_even);
    printf("Even values only: ");
    list_print(evens);
    
    printf("\n8. Complex type example:\n");
    LinkedList *items = create_linked_list(sizeof(Item), compare_item_by_id, 
                                          print_item, NULL);
    
    Item item_data[] = {
        {3, "Widget", 19.99},
        {1, "Gadget", 29.99},
        {4, "Doohickey", 14.99},
        {2, "Thingamajig", 24.99}
    };
    
    for (int i = 0; i < 4; i++) {
        list_push_back(items, &item_data[i]);
    }
    
    printf("Items unsorted: ");
    list_print(items);
    
    list_sort(items);
    printf("Items sorted by ID: ");
    list_print(items);
    
    printf("\n9. Get at index:\n");
    for (size_t i = 0; i < items->size; i++) {
        Item *item = (Item *)list_get_at(items, i);
        printf("Item at index %zu: ", i);
        print_item(item);
        printf("\n");
    }
    
    // Cleanup
    free_linked_list(list);
    free_linked_list(squared);
    free_linked_list(evens);
    free_linked_list(items);
    
    return 0;
}