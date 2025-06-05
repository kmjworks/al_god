#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

// Basic tree node structure
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// AVL tree node structure
typedef struct AVLNode {
    int data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

// Helper functions
TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) return NULL;
    
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void printSpaces(int count) {
    for (int i = 0; i < count; i++) {
        printf(" ");
    }
}

// Pretty print tree
void printTreeHelper(TreeNode* root, int space) {
    if (root == NULL) return;
    
    space += 5;
    
    printTreeHelper(root->right, space);
    
    printf("\n");
    printSpaces(space - 5);
    printf("%d\n", root->data);
    
    printTreeHelper(root->left, space);
}

void printTree(TreeNode* root) {
    printTreeHelper(root, 0);
    printf("\n");
}

// Tree traversal demonstrations
void traversal_demo() {
    printf("\n=== Tree Traversal Demo ===\n");
    
    // Create a sample tree
    //        1
    //       / \
    //      2   3
    //     / \
    //    4   5
    
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    
    printf("Tree structure:\n");
    printTree(root);
    
    // Inorder traversal
    printf("Inorder traversal: ");
    void inorder(TreeNode* node) {
        if (node == NULL) return;
        inorder(node->left);
        printf("%d ", node->data);
        inorder(node->right);
    }
    inorder(root);
    printf("\n");
    
    // Preorder traversal
    printf("Preorder traversal: ");
    void preorder(TreeNode* node) {
        if (node == NULL) return;
        printf("%d ", node->data);
        preorder(node->left);
        preorder(node->right);
    }
    preorder(root);
    printf("\n");
    
    // Postorder traversal
    printf("Postorder traversal: ");
    void postorder(TreeNode* node) {
        if (node == NULL) return;
        postorder(node->left);
        postorder(node->right);
        printf("%d ", node->data);
    }
    postorder(root);
    printf("\n");
    
    // Level order traversal
    printf("Level order traversal: ");
    TreeNode* queue[100];
    int front = 0, rear = 0;
    
    if (root != NULL) {
        queue[rear++] = root;
        
        while (front < rear) {
            TreeNode* current = queue[front++];
            printf("%d ", current->data);
            
            if (current->left) queue[rear++] = current->left;
            if (current->right) queue[rear++] = current->right;
        }
    }
    printf("\n");
    
    // Clean up
    free(root->left->left);
    free(root->left->right);
    free(root->left);
    free(root->right);
    free(root);
}

// BST operations demonstration
TreeNode* insertBST(TreeNode* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    
    if (data < root->data) {
        root->left = insertBST(root->left, data);
    } else if (data > root->data) {
        root->right = insertBST(root->right, data);
    }
    
    return root;
}

TreeNode* searchBST(TreeNode* root, int key) {
    if (root == NULL || root->data == key) {
        return root;
    }
    
    if (key < root->data) {
        return searchBST(root->left, key);
    }
    
    return searchBST(root->right, key);
}

TreeNode* findMin(TreeNode* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

TreeNode* deleteBST(TreeNode* root, int key) {
    if (root == NULL) return NULL;
    
    if (key < root->data) {
        root->left = deleteBST(root->left, key);
    } else if (key > root->data) {
        root->right = deleteBST(root->right, key);
    } else {
        // Node to delete found
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }
        
        // Two children
        TreeNode* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteBST(root->right, temp->data);
    }
    
    return root;
}

void bst_demo() {
    printf("\n=== Binary Search Tree Demo ===\n");
    
    TreeNode* bst = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    
    // Insert values
    printf("Inserting values: ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", values[i]);
        bst = insertBST(bst, values[i]);
    }
    printf("\n\nBST structure:\n");
    printTree(bst);
    
    // Search operations
    int searchKeys[] = {40, 25, 70};
    for (int i = 0; i < 3; i++) {
        TreeNode* found = searchBST(bst, searchKeys[i]);
        printf("Search for %d: %s\n", searchKeys[i], 
               found ? "Found" : "Not found");
    }
    
    // Find min and max
    TreeNode* minNode = findMin(bst);
    TreeNode* maxNode = bst;
    while (maxNode->right != NULL) {
        maxNode = maxNode->right;
    }
    printf("\nMinimum value: %d\n", minNode->data);
    printf("Maximum value: %d\n", maxNode->data);
    
    // Delete operations
    printf("\nDeleting 20 (leaf node):\n");
    bst = deleteBST(bst, 20);
    printTree(bst);
    
    printf("Deleting 30 (node with one child):\n");
    bst = deleteBST(bst, 30);
    printTree(bst);
    
    printf("Deleting 50 (node with two children):\n");
    bst = deleteBST(bst, 50);
    printTree(bst);
    
    // Clean up (simplified - in practice, use proper tree deletion)
}

// Tree properties demonstration
int height(TreeNode* root) {
    if (root == NULL) return -1;
    
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int countNodes(TreeNode* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeaves(TreeNode* root) {
    if (root == NULL) return 0;
    if (root->left == NULL && root->right == NULL) return 1;
    return countLeaves(root->left) + countLeaves(root->right);
}

bool isBSTUtil(TreeNode* root, int min, int max) {
    if (root == NULL) return true;
    
    if (root->data <= min || root->data >= max) return false;
    
    return isBSTUtil(root->left, min, root->data) &&
           isBSTUtil(root->right, root->data, max);
}

bool isBST(TreeNode* root) {
    return isBSTUtil(root, INT_MIN, INT_MAX);
}

void tree_properties_demo() {
    printf("\n=== Tree Properties Demo ===\n");
    
    // Create a BST
    TreeNode* tree = createNode(10);
    tree->left = createNode(5);
    tree->right = createNode(15);
    tree->left->left = createNode(3);
    tree->left->right = createNode(7);
    tree->right->right = createNode(20);
    
    printf("Tree structure:\n");
    printTree(tree);
    
    printf("Tree properties:\n");
    printf("Height: %d\n", height(tree));
    printf("Total nodes: %d\n", countNodes(tree));
    printf("Leaf nodes: %d\n", countLeaves(tree));
    printf("Is BST: %s\n", isBST(tree) ? "Yes" : "No");
    
    // Create a non-BST
    TreeNode* nonBST = createNode(10);
    nonBST->left = createNode(5);
    nonBST->right = createNode(15);
    nonBST->left->right = createNode(12);  // Violates BST property
    
    printf("\nNon-BST structure:\n");
    printTree(nonBST);
    printf("Is BST: %s\n", isBST(nonBST) ? "Yes" : "No");
}

// Advanced operations
TreeNode* findLCA(TreeNode* root, int n1, int n2) {
    if (root == NULL) return NULL;
    
    if (root->data == n1 || root->data == n2) return root;
    
    TreeNode* leftLCA = findLCA(root->left, n1, n2);
    TreeNode* rightLCA = findLCA(root->right, n1, n2);
    
    if (leftLCA && rightLCA) return root;
    
    return (leftLCA != NULL) ? leftLCA : rightLCA;
}

void mirror(TreeNode* root) {
    if (root == NULL) return;
    
    TreeNode* temp = root->left;
    root->left = root->right;
    root->right = temp;
    
    mirror(root->left);
    mirror(root->right);
}

bool hasPathSum(TreeNode* root, int sum) {
    if (root == NULL) return false;
    
    if (root->left == NULL && root->right == NULL) {
        return root->data == sum;
    }
    
    return hasPathSum(root->left, sum - root->data) ||
           hasPathSum(root->right, sum - root->data);
}

void advanced_operations_demo() {
    printf("\n=== Advanced Operations Demo ===\n");
    
    // Create tree for LCA
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->left = createNode(6);
    root->right->right = createNode(7);
    
    printf("Tree for LCA:\n");
    printTree(root);
    
    TreeNode* lca = findLCA(root, 4, 5);
    printf("LCA of 4 and 5: %d\n", lca ? lca->data : -1);
    
    lca = findLCA(root, 4, 7);
    printf("LCA of 4 and 7: %d\n", lca ? lca->data : -1);
    
    // Path sum
    int targetSum = 7;
    printf("\nPath sum = %d exists: %s\n", targetSum,
           hasPathSum(root, targetSum) ? "Yes" : "No");
    
    // Mirror tree
    printf("\nOriginal tree:\n");
    printTree(root);
    
    mirror(root);
    printf("Mirrored tree:\n");
    printTree(root);
}

// AVL Tree operations
AVLNode* createAVLNode(int data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->data = data;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

int getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

void updateHeight(AVLNode* node) {
    if (node) {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }
}

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    updateHeight(y);
    updateHeight(x);
    
    return x;
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    updateHeight(x);
    updateHeight(y);
    
    return y;
}

AVLNode* insertAVL(AVLNode* root, int data) {
    if (root == NULL) return createAVLNode(data);
    
    if (data < root->data) {
        root->left = insertAVL(root->left, data);
    } else if (data > root->data) {
        root->right = insertAVL(root->right, data);
    } else {
        return root;
    }
    
    updateHeight(root);
    
    int balance = getBalance(root);
    
    // Left-Left
    if (balance > 1 && data < root->left->data) {
        return rotateRight(root);
    }
    
    // Right-Right
    if (balance < -1 && data > root->right->data) {
        return rotateLeft(root);
    }
    
    // Left-Right
    if (balance > 1 && data > root->left->data) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    
    // Right-Left
    if (balance < -1 && data < root->right->data) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    
    return root;
}

void printAVLTree(AVLNode* root, int space) {
    if (root == NULL) return;
    
    space += 5;
    
    printAVLTree(root->right, space);
    
    printf("\n");
    printSpaces(space - 5);
    printf("%d(%d)\n", root->data, root->height);
    
    printAVLTree(root->left, space);
}

void avl_demo() {
    printf("\n=== AVL Tree Demo ===\n");
    
    AVLNode* avl = NULL;
    int values[] = {10, 20, 30, 40, 50, 25};
    
    printf("Inserting values into AVL tree:\n");
    for (int i = 0; i < 6; i++) {
        printf("\nInserting %d:\n", values[i]);
        avl = insertAVL(avl, values[i]);
        printAVLTree(avl, 0);
    }
    
    printf("\nFinal AVL tree (balanced):\n");
    printAVLTree(avl, 0);
}

// Tree construction from traversals
TreeNode* buildTreeInPre(int inorder[], int preorder[], 
                         int inStart, int inEnd, int* preIndex) {
    if (inStart > inEnd) return NULL;
    
    TreeNode* node = createNode(preorder[(*preIndex)++]);
    
    if (inStart == inEnd) return node;
    
    int inIndex;
    for (inIndex = inStart; inIndex <= inEnd; inIndex++) {
        if (inorder[inIndex] == node->data) break;
    }
    
    node->left = buildTreeInPre(inorder, preorder, inStart, inIndex - 1, preIndex);
    node->right = buildTreeInPre(inorder, preorder, inIndex + 1, inEnd, preIndex);
    
    return node;
}

void tree_construction_demo() {
    printf("\n=== Tree Construction from Traversals ===\n");
    
    int inorder[] = {4, 2, 5, 1, 6, 3, 7};
    int preorder[] = {1, 2, 4, 5, 3, 6, 7};
    int n = 7;
    
    printf("Inorder: ");
    for (int i = 0; i < n; i++) printf("%d ", inorder[i]);
    printf("\nPreorder: ");
    for (int i = 0; i < n; i++) printf("%d ", preorder[i]);
    
    int preIndex = 0;
    TreeNode* root = buildTreeInPre(inorder, preorder, 0, n - 1, &preIndex);
    
    printf("\n\nReconstructed tree:\n");
    printTree(root);
}

// Level-wise operations
void printLevelOrder(TreeNode* root) {
    if (root == NULL) return;
    
    TreeNode* queue[100];
    int front = 0, rear = 0;
    
    queue[rear++] = root;
    queue[rear++] = NULL;  // Level marker
    
    while (front < rear) {
        TreeNode* current = queue[front++];
        
        if (current == NULL) {
            printf("\n");
            if (front < rear) {
                queue[rear++] = NULL;
            }
        } else {
            printf("%d ", current->data);
            if (current->left) queue[rear++] = current->left;
            if (current->right) queue[rear++] = current->right;
        }
    }
}

void level_operations_demo() {
    printf("\n=== Level-wise Operations Demo ===\n");
    
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->left = createNode(6);
    root->right->right = createNode(7);
    
    printf("Level-order traversal with levels:\n");
    printLevelOrder(root);
}

int main() {
    printf("BINARY TREES COMPREHENSIVE EXAMPLES\n");
    printf("===================================\n");
    
    traversal_demo();
    bst_demo();
    tree_properties_demo();
    advanced_operations_demo();
    avl_demo();
    tree_construction_demo();
    level_operations_demo();
    
    printf("\nAll demonstrations completed!\n");
    
    return 0;
}