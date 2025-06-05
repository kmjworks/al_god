#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generic binary tree node
typedef struct TreeNode {
    void *data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Generic binary tree structure
typedef struct {
    TreeNode *root;
    size_t data_size;
    int (*compare)(const void *, const void *);
    void (*print)(const void *);
    void (*free_data)(void *);
} BinaryTree;

// Create a new tree node
TreeNode* create_node(void *data, size_t data_size) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (!node) return NULL;
    
    node->data = malloc(data_size);
    if (!node->data) {
        free(node);
        return NULL;
    }
    
    memcpy(node->data, data, data_size);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Initialize binary tree
BinaryTree* create_tree(size_t data_size, 
                       int (*compare)(const void *, const void *),
                       void (*print)(const void *),
                       void (*free_data)(void *)) {
    BinaryTree *tree = (BinaryTree *)malloc(sizeof(BinaryTree));
    if (!tree) return NULL;
    
    tree->root = NULL;
    tree->data_size = data_size;
    tree->compare = compare;
    tree->print = print;
    tree->free_data = free_data;
    return tree;
}

// Insert helper
TreeNode* insert_helper(TreeNode *node, void *data, size_t data_size,
                       int (*compare)(const void *, const void *)) {
    if (!node) {
        return create_node(data, data_size);
    }
    
    int cmp = compare(data, node->data);
    if (cmp < 0) {
        node->left = insert_helper(node->left, data, data_size, compare);
    } else if (cmp > 0) {
        node->right = insert_helper(node->right, data, data_size, compare);
    }
    
    return node;
}

// Insert into tree
void tree_insert(BinaryTree *tree, void *data) {
    tree->root = insert_helper(tree->root, data, tree->data_size, tree->compare);
}

// Search helper
TreeNode* search_helper(TreeNode *node, void *data,
                       int (*compare)(const void *, const void *)) {
    if (!node) return NULL;
    
    int cmp = compare(data, node->data);
    if (cmp == 0) return node;
    if (cmp < 0) return search_helper(node->left, data, compare);
    return search_helper(node->right, data, compare);
}

// Search in tree
void* tree_search(BinaryTree *tree, void *data) {
    TreeNode *node = search_helper(tree->root, data, tree->compare);
    return node ? node->data : NULL;
}

// Find minimum node
TreeNode* find_min(TreeNode *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

// Delete helper
TreeNode* delete_helper(TreeNode *node, void *data, size_t data_size,
                       int (*compare)(const void *, const void *),
                       void (*free_data)(void *)) {
    if (!node) return NULL;
    
    int cmp = compare(data, node->data);
    
    if (cmp < 0) {
        node->left = delete_helper(node->left, data, data_size, compare, free_data);
    } else if (cmp > 0) {
        node->right = delete_helper(node->right, data, data_size, compare, free_data);
    } else {
        // Node with only one child or no child
        if (!node->left) {
            TreeNode *temp = node->right;
            if (free_data) free_data(node->data);
            else free(node->data);
            free(node);
            return temp;
        } else if (!node->right) {
            TreeNode *temp = node->left;
            if (free_data) free_data(node->data);
            else free(node->data);
            free(node);
            return temp;
        }
        
        // Node with two children
        TreeNode *temp = find_min(node->right);
        memcpy(node->data, temp->data, data_size);
        node->right = delete_helper(node->right, temp->data, data_size, compare, free_data);
    }
    
    return node;
}

// Delete from tree
void tree_delete(BinaryTree *tree, void *data) {
    tree->root = delete_helper(tree->root, data, tree->data_size, 
                              tree->compare, tree->free_data);
}

// In-order traversal
void inorder_helper(TreeNode *node, void (*print)(const void *)) {
    if (!node) return;
    
    inorder_helper(node->left, print);
    print(node->data);
    printf(" ");
    inorder_helper(node->right, print);
}

void tree_inorder(BinaryTree *tree) {
    inorder_helper(tree->root, tree->print);
    printf("\n");
}

// Pre-order traversal
void preorder_helper(TreeNode *node, void (*print)(const void *)) {
    if (!node) return;
    
    print(node->data);
    printf(" ");
    preorder_helper(node->left, print);
    preorder_helper(node->right, print);
}

void tree_preorder(BinaryTree *tree) {
    preorder_helper(tree->root, tree->print);
    printf("\n");
}

// Post-order traversal
void postorder_helper(TreeNode *node, void (*print)(const void *)) {
    if (!node) return;
    
    postorder_helper(node->left, print);
    postorder_helper(node->right, print);
    print(node->data);
    printf(" ");
}

void tree_postorder(BinaryTree *tree) {
    postorder_helper(tree->root, tree->print);
    printf("\n");
}

// Calculate height
int height_helper(TreeNode *node) {
    if (!node) return -1;
    
    int left_height = height_helper(node->left);
    int right_height = height_helper(node->right);
    
    return 1 + (left_height > right_height ? left_height : right_height);
}

int tree_height(BinaryTree *tree) {
    return height_helper(tree->root);
}

// Count nodes
int count_helper(TreeNode *node) {
    if (!node) return 0;
    return 1 + count_helper(node->left) + count_helper(node->right);
}

int tree_count(BinaryTree *tree) {
    return count_helper(tree->root);
}

// Free tree helper
void free_tree_helper(TreeNode *node, void (*free_data)(void *)) {
    if (!node) return;
    
    free_tree_helper(node->left, free_data);
    free_tree_helper(node->right, free_data);
    
    if (free_data) free_data(node->data);
    else free(node->data);
    free(node);
}

// Free entire tree
void free_tree(BinaryTree *tree) {
    free_tree_helper(tree->root, tree->free_data);
    free(tree);
}

// Comparison and print functions for different types
int compare_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void print_int(const void *data) {
    printf("%d", *(int *)data);
}

int compare_string(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

void print_string(const void *data) {
    printf("%s", *(char **)data);
}

void free_string(void *data) {
    free(*(char **)data);
    free(data);
}

// Complex structure example
typedef struct {
    int id;
    double value;
} Record;

int compare_record(const void *a, const void *b) {
    Record *r1 = (Record *)a;
    Record *r2 = (Record *)b;
    return r1->id - r2->id;
}

void print_record(const void *data) {
    Record *r = (Record *)data;
    printf("(%d:%.2f)", r->id, r->value);
}

// Example usage
int main() {
    // Example 1: Integer binary tree
    printf("=== Integer Binary Tree ===\n");
    BinaryTree *int_tree = create_tree(sizeof(int), compare_int, print_int, NULL);
    
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) {
        tree_insert(int_tree, &values[i]);
    }
    
    printf("In-order: ");
    tree_inorder(int_tree);
    printf("Pre-order: ");
    tree_preorder(int_tree);
    printf("Post-order: ");
    tree_postorder(int_tree);
    
    printf("Height: %d\n", tree_height(int_tree));
    printf("Node count: %d\n", tree_count(int_tree));
    
    int search_val = 40;
    int *found = (int *)tree_search(int_tree, &search_val);
    printf("Search for %d: %s\n", search_val, found ? "Found" : "Not found");
    
    int del_val = 30;
    printf("Deleting %d\n", del_val);
    tree_delete(int_tree, &del_val);
    printf("In-order after deletion: ");
    tree_inorder(int_tree);
    
    free_tree(int_tree);
    
    // Example 2: String binary tree
    printf("\n=== String Binary Tree ===\n");
    BinaryTree *str_tree = create_tree(sizeof(char *), compare_string, 
                                      print_string, free_string);
    
    char *words[] = {"dog", "cat", "elephant", "bird", "fish", "ant"};
    for (int i = 0; i < 6; i++) {
        char *word = strdup(words[i]);
        tree_insert(str_tree, &word);
    }
    
    printf("In-order: ");
    tree_inorder(str_tree);
    
    free_tree(str_tree);
    
    // Example 3: Record binary tree
    printf("\n=== Record Binary Tree ===\n");
    BinaryTree *rec_tree = create_tree(sizeof(Record), compare_record, 
                                      print_record, NULL);
    
    Record records[] = {{5, 5.5}, {3, 3.3}, {7, 7.7}, {1, 1.1}, 
                       {4, 4.4}, {6, 6.6}, {8, 8.8}};
    for (int i = 0; i < 7; i++) {
        tree_insert(rec_tree, &records[i]);
    }
    
    printf("In-order: ");
    tree_inorder(rec_tree);
    
    Record search_rec = {4, 0.0};
    Record *found_rec = (Record *)tree_search(rec_tree, &search_rec);
    if (found_rec) {
        printf("Found record: ID=%d, Value=%.2f\n", found_rec->id, found_rec->value);
    }
    
    free_tree(rec_tree);
    
    return 0;
}