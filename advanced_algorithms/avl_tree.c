#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// AVL Tree Node
typedef struct AVLNode {
    void *data;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// AVL Tree structure
typedef struct {
    AVLNode *root;
    size_t data_size;
    int (*compare)(const void *, const void *);
    void (*print)(const void *);
} AVLTree;

// Get height of node
int height(AVLNode *node) {
    return node ? node->height : 0;
}

// Get balance factor
int get_balance(AVLNode *node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Update height
void update_height(AVLNode *node) {
    if (node) {
        int left_height = height(node->left);
        int right_height = height(node->right);
        node->height = 1 + (left_height > right_height ? left_height : right_height);
    }
}

// Create new node
AVLNode* create_avl_node(void *data, size_t data_size) {
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (!node) return NULL;
    
    node->data = malloc(data_size);
    if (!node->data) {
        free(node);
        return NULL;
    }
    
    memcpy(node->data, data, data_size);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Right rotate
AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    update_height(y);
    update_height(x);
    
    return x;
}

// Left rotate
AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    update_height(x);
    update_height(y);
    
    return y;
}

// Insert helper with balancing
AVLNode* avl_insert_helper(AVLNode *node, void *data, size_t data_size,
                          int (*compare)(const void *, const void *)) {
    // Step 1: Perform normal BST insertion
    if (!node) {
        return create_avl_node(data, data_size);
    }
    
    int cmp = compare(data, node->data);
    
    if (cmp < 0) {
        node->left = avl_insert_helper(node->left, data, data_size, compare);
    } else if (cmp > 0) {
        node->right = avl_insert_helper(node->right, data, data_size, compare);
    } else {
        // Equal keys not allowed
        return node;
    }
    
    // Step 2: Update height
    update_height(node);
    
    // Step 3: Get balance factor
    int balance = get_balance(node);
    
    // Step 4: If unbalanced, there are 4 cases
    
    // Left Left Case
    if (balance > 1 && compare(data, node->left->data) < 0) {
        return rotate_right(node);
    }
    
    // Right Right Case
    if (balance < -1 && compare(data, node->right->data) > 0) {
        return rotate_left(node);
    }
    
    // Left Right Case
    if (balance > 1 && compare(data, node->left->data) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    
    // Right Left Case
    if (balance < -1 && compare(data, node->right->data) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    
    return node;
}

// Find minimum node
AVLNode* find_min_avl(AVLNode *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

// Delete helper with balancing
AVLNode* avl_delete_helper(AVLNode *node, void *data, size_t data_size,
                          int (*compare)(const void *, const void *)) {
    // Step 1: Perform standard BST delete
    if (!node) return NULL;
    
    int cmp = compare(data, node->data);
    
    if (cmp < 0) {
        node->left = avl_delete_helper(node->left, data, data_size, compare);
    } else if (cmp > 0) {
        node->right = avl_delete_helper(node->right, data, data_size, compare);
    } else {
        // Node with only one child or no child
        if (!node->left) {
            AVLNode *temp = node->right;
            free(node->data);
            free(node);
            return temp;
        } else if (!node->right) {
            AVLNode *temp = node->left;
            free(node->data);
            free(node);
            return temp;
        }
        
        // Node with two children
        AVLNode *temp = find_min_avl(node->right);
        memcpy(node->data, temp->data, data_size);
        node->right = avl_delete_helper(node->right, temp->data, data_size, compare);
    }
    
    if (!node) return node;
    
    // Step 2: Update height
    update_height(node);
    
    // Step 3: Get balance factor
    int balance = get_balance(node);
    
    // Step 4: If unbalanced, there are 4 cases
    
    // Left Left Case
    if (balance > 1 && get_balance(node->left) >= 0) {
        return rotate_right(node);
    }
    
    // Left Right Case
    if (balance > 1 && get_balance(node->left) < 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    
    // Right Right Case
    if (balance < -1 && get_balance(node->right) <= 0) {
        return rotate_left(node);
    }
    
    // Right Left Case
    if (balance < -1 && get_balance(node->right) > 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    
    return node;
}

// Create AVL tree
AVLTree* create_avl_tree(size_t data_size,
                        int (*compare)(const void *, const void *),
                        void (*print)(const void *)) {
    AVLTree *tree = (AVLTree *)malloc(sizeof(AVLTree));
    if (!tree) return NULL;
    
    tree->root = NULL;
    tree->data_size = data_size;
    tree->compare = compare;
    tree->print = print;
    return tree;
}

// Public insert function
void avl_insert(AVLTree *tree, void *data) {
    tree->root = avl_insert_helper(tree->root, data, tree->data_size, tree->compare);
}

// Public delete function
void avl_delete(AVLTree *tree, void *data) {
    tree->root = avl_delete_helper(tree->root, data, tree->data_size, tree->compare);
}

// Print tree structure (for visualization)
void print_tree_helper(AVLNode *node, void (*print)(const void *), int space) {
    if (!node) return;
    
    space += 5;
    
    print_tree_helper(node->right, print, space);
    
    printf("\n");
    for (int i = 5; i < space; i++) printf(" ");
    print(node->data);
    printf("(%d)", node->height);
    
    print_tree_helper(node->left, print, space);
}

void print_avl_tree(AVLTree *tree) {
    printf("AVL Tree Structure:\n");
    print_tree_helper(tree->root, tree->print, 0);
    printf("\n");
}

// In-order traversal
void avl_inorder_helper(AVLNode *node, void (*print)(const void *)) {
    if (!node) return;
    
    avl_inorder_helper(node->left, print);
    print(node->data);
    printf(" ");
    avl_inorder_helper(node->right, print);
}

void avl_inorder(AVLTree *tree) {
    avl_inorder_helper(tree->root, tree->print);
    printf("\n");
}

// Check if tree is balanced
int is_balanced_helper(AVLNode *node) {
    if (!node) return 1;
    
    int balance = get_balance(node);
    if (balance > 1 || balance < -1) return 0;
    
    return is_balanced_helper(node->left) && is_balanced_helper(node->right);
}

int is_balanced(AVLTree *tree) {
    return is_balanced_helper(tree->root);
}

// Free tree
void free_avl_helper(AVLNode *node) {
    if (!node) return;
    
    free_avl_helper(node->left);
    free_avl_helper(node->right);
    free(node->data);
    free(node);
}

void free_avl_tree(AVLTree *tree) {
    free_avl_helper(tree->root);
    free(tree);
}

// Comparison and print functions
int compare_int(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void print_int(const void *data) {
    printf("%d", *(int *)data);
}

// Advanced example: Self-balancing during bulk operations
void demonstrate_rotations() {
    printf("=== AVL Tree Rotation Demonstration ===\n");
    AVLTree *tree = create_avl_tree(sizeof(int), compare_int, print_int);
    
    printf("\n1. Left-Left Case (Right Rotation):\n");
    int ll_case[] = {30, 20, 10};
    for (int i = 0; i < 3; i++) {
        printf("Inserting %d:\n", ll_case[i]);
        avl_insert(tree, &ll_case[i]);
        print_avl_tree(tree);
    }
    
    free_avl_tree(tree);
    tree = create_avl_tree(sizeof(int), compare_int, print_int);
    
    printf("\n2. Right-Right Case (Left Rotation):\n");
    int rr_case[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        printf("Inserting %d:\n", rr_case[i]);
        avl_insert(tree, &rr_case[i]);
        print_avl_tree(tree);
    }
    
    free_avl_tree(tree);
    tree = create_avl_tree(sizeof(int), compare_int, print_int);
    
    printf("\n3. Left-Right Case (Left-Right Rotation):\n");
    int lr_case[] = {30, 10, 20};
    for (int i = 0; i < 3; i++) {
        printf("Inserting %d:\n", lr_case[i]);
        avl_insert(tree, &lr_case[i]);
        print_avl_tree(tree);
    }
    
    free_avl_tree(tree);
    tree = create_avl_tree(sizeof(int), compare_int, print_int);
    
    printf("\n4. Right-Left Case (Right-Left Rotation):\n");
    int rl_case[] = {10, 30, 20};
    for (int i = 0; i < 3; i++) {
        printf("Inserting %d:\n", rl_case[i]);
        avl_insert(tree, &rl_case[i]);
        print_avl_tree(tree);
    }
    
    free_avl_tree(tree);
}

// Main demonstration
int main() {
    // Basic AVL tree operations
    printf("=== AVL Tree Operations ===\n");
    AVLTree *tree = create_avl_tree(sizeof(int), compare_int, print_int);
    
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35};
    int n = sizeof(values) / sizeof(values[0]);
    
    printf("Inserting values: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", values[i]);
        avl_insert(tree, &values[i]);
    }
    printf("\n");
    
    print_avl_tree(tree);
    
    printf("\nIn-order traversal: ");
    avl_inorder(tree);
    
    printf("Tree height: %d\n", height(tree->root));
    printf("Is balanced: %s\n", is_balanced(tree) ? "Yes" : "No");
    
    // Deletion demonstration
    printf("\n=== Deletion Operations ===\n");
    int delete_vals[] = {20, 30, 50};
    for (int i = 0; i < 3; i++) {
        printf("\nDeleting %d:\n", delete_vals[i]);
        avl_delete(tree, &delete_vals[i]);
        print_avl_tree(tree);
        printf("Is balanced: %s\n", is_balanced(tree) ? "Yes" : "No");
    }
    
    free_avl_tree(tree);
    
    // Demonstrate all rotation types
    demonstrate_rotations();
    
    // Performance comparison
    printf("\n=== Performance Comparison ===\n");
    printf("Inserting 15 sequential values:\n");
    tree = create_avl_tree(sizeof(int), compare_int, print_int);
    
    for (int i = 1; i <= 15; i++) {
        avl_insert(tree, &i);
    }
    
    printf("AVL Tree height: %d (log2(15) ≈ 3.9)\n", height(tree->root));
    printf("Is balanced: %s\n", is_balanced(tree) ? "Yes" : "No");
    print_avl_tree(tree);
    
    free_avl_tree(tree);
    
    return 0;
}