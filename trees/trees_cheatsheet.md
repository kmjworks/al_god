# Trees (Binary Trees, BST, AVL) Cheat Sheet - C Programming

## What is a Tree?
A tree is a hierarchical data structure consisting of nodes connected by edges. Each node contains data and references to its child nodes.

## Tree Terminology
- **Root**: Top node of the tree
- **Parent**: Node that has children
- **Child**: Node connected below another node
- **Leaf**: Node with no children
- **Height**: Length of longest path from node to leaf
- **Depth**: Length of path from root to node
- **Level**: Depth + 1
- **Subtree**: Tree formed by a node and its descendants

## Types of Trees

### 1. Binary Tree
Each node has at most two children (left and right).

```c
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;
```

### 2. Binary Search Tree (BST)
Binary tree where left child < parent < right child.

### 3. AVL Tree
Self-balancing BST where heights of subtrees differ by at most 1.

### 4. Complete Binary Tree
All levels are filled except possibly the last, which is filled left to right.

### 5. Full Binary Tree
Every node has either 0 or 2 children.

## Basic Binary Tree Operations

### Node Creation
```c
TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
```

### Tree Traversals

#### 1. Inorder Traversal (Left-Root-Right)
```c
void inorder(TreeNode* root) {
    if (root == NULL) return;
    
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

// Iterative inorder using stack
void inorderIterative(TreeNode* root) {
    if (root == NULL) return;
    
    TreeNode* stack[100];
    int top = -1;
    TreeNode* current = root;
    
    while (current != NULL || top >= 0) {
        while (current != NULL) {
            stack[++top] = current;
            current = current->left;
        }
        
        current = stack[top--];
        printf("%d ", current->data);
        current = current->right;
    }
}
```

#### 2. Preorder Traversal (Root-Left-Right)
```c
void preorder(TreeNode* root) {
    if (root == NULL) return;
    
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

// Iterative preorder
void preorderIterative(TreeNode* root) {
    if (root == NULL) return;
    
    TreeNode* stack[100];
    int top = -1;
    stack[++top] = root;
    
    while (top >= 0) {
        TreeNode* current = stack[top--];
        printf("%d ", current->data);
        
        if (current->right) stack[++top] = current->right;
        if (current->left) stack[++top] = current->left;
    }
}
```

#### 3. Postorder Traversal (Left-Right-Root)
```c
void postorder(TreeNode* root) {
    if (root == NULL) return;
    
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}
```

#### 4. Level Order Traversal (BFS)
```c
void levelOrder(TreeNode* root) {
    if (root == NULL) return;
    
    TreeNode* queue[100];
    int front = 0, rear = 0;
    
    queue[rear++] = root;
    
    while (front < rear) {
        TreeNode* current = queue[front++];
        printf("%d ", current->data);
        
        if (current->left) queue[rear++] = current->left;
        if (current->right) queue[rear++] = current->right;
    }
}

// Level order with level separation
void levelOrderWithLevels(TreeNode* root) {
    if (root == NULL) return;
    
    TreeNode* queue[100];
    int front = 0, rear = 0;
    
    queue[rear++] = root;
    queue[rear++] = NULL;  // Level separator
    
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
```

## Binary Search Tree (BST) Operations

### BST Insertion
```c
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

// Iterative insertion
TreeNode* insertBSTIterative(TreeNode* root, int data) {
    TreeNode* newNode = createNode(data);
    if (root == NULL) return newNode;
    
    TreeNode* current = root;
    TreeNode* parent = NULL;
    
    while (current != NULL) {
        parent = current;
        if (data < current->data) {
            current = current->left;
        } else if (data > current->data) {
            current = current->right;
        } else {
            free(newNode);  // Duplicate
            return root;
        }
    }
    
    if (data < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    
    return root;
}
```

### BST Search
```c
TreeNode* searchBST(TreeNode* root, int key) {
    if (root == NULL || root->data == key) {
        return root;
    }
    
    if (key < root->data) {
        return searchBST(root->left, key);
    }
    
    return searchBST(root->right, key);
}

// Iterative search
TreeNode* searchBSTIterative(TreeNode* root, int key) {
    while (root != NULL && root->data != key) {
        if (key < root->data) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return root;
}
```

### Find Minimum and Maximum
```c
TreeNode* findMin(TreeNode* root) {
    if (root == NULL) return NULL;
    
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

TreeNode* findMax(TreeNode* root) {
    if (root == NULL) return NULL;
    
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}
```

### BST Deletion
```c
TreeNode* deleteBST(TreeNode* root, int key) {
    if (root == NULL) return NULL;
    
    if (key < root->data) {
        root->left = deleteBST(root->left, key);
    } else if (key > root->data) {
        root->right = deleteBST(root->right, key);
    } else {
        // Node to be deleted found
        
        // Case 1: Leaf node
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        
        // Case 2: One child
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }
        
        // Case 3: Two children
        TreeNode* minRight = findMin(root->right);
        root->data = minRight->data;
        root->right = deleteBST(root->right, minRight->data);
    }
    
    return root;
}
```

## Tree Properties and Checks

### Height of Tree
```c
int height(TreeNode* root) {
    if (root == NULL) {
        return -1;  // or 0, depending on definition
    }
    
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}
```

### Count Nodes
```c
int countNodes(TreeNode* root) {
    if (root == NULL) return 0;
    
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Count leaf nodes
int countLeaves(TreeNode* root) {
    if (root == NULL) return 0;
    
    if (root->left == NULL && root->right == NULL) {
        return 1;
    }
    
    return countLeaves(root->left) + countLeaves(root->right);
}
```

### Check if BST
```c
bool isBSTUtil(TreeNode* root, int min, int max) {
    if (root == NULL) return true;
    
    if (root->data <= min || root->data >= max) {
        return false;
    }
    
    return isBSTUtil(root->left, min, root->data) &&
           isBSTUtil(root->right, root->data, max);
}

bool isBST(TreeNode* root) {
    return isBSTUtil(root, INT_MIN, INT_MAX);
}

// Alternative: Inorder traversal approach
bool isBSTInorder(TreeNode* root) {
    static TreeNode* prev = NULL;
    
    if (root == NULL) return true;
    
    if (!isBSTInorder(root->left)) return false;
    
    if (prev != NULL && root->data <= prev->data) {
        return false;
    }
    prev = root;
    
    return isBSTInorder(root->right);
}
```

### Check if Balanced
```c
int checkHeight(TreeNode* root) {
    if (root == NULL) return 0;
    
    int leftHeight = checkHeight(root->left);
    if (leftHeight == -1) return -1;
    
    int rightHeight = checkHeight(root->right);
    if (rightHeight == -1) return -1;
    
    int diff = abs(leftHeight - rightHeight);
    if (diff > 1) return -1;
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

bool isBalanced(TreeNode* root) {
    return checkHeight(root) != -1;
}
```

## Advanced Tree Operations

### Lowest Common Ancestor (LCA)
```c
// For Binary Tree
TreeNode* findLCA(TreeNode* root, int n1, int n2) {
    if (root == NULL) return NULL;
    
    if (root->data == n1 || root->data == n2) {
        return root;
    }
    
    TreeNode* leftLCA = findLCA(root->left, n1, n2);
    TreeNode* rightLCA = findLCA(root->right, n1, n2);
    
    if (leftLCA && rightLCA) return root;
    
    return (leftLCA != NULL) ? leftLCA : rightLCA;
}

// For BST
TreeNode* findLCABST(TreeNode* root, int n1, int n2) {
    if (root == NULL) return NULL;
    
    if (root->data > n1 && root->data > n2) {
        return findLCABST(root->left, n1, n2);
    }
    
    if (root->data < n1 && root->data < n2) {
        return findLCABST(root->right, n1, n2);
    }
    
    return root;
}
```

### Path from Root to Node
```c
bool findPath(TreeNode* root, int key, int path[], int* pathLen) {
    if (root == NULL) return false;
    
    path[(*pathLen)++] = root->data;
    
    if (root->data == key) return true;
    
    if (findPath(root->left, key, path, pathLen) ||
        findPath(root->right, key, path, pathLen)) {
        return true;
    }
    
    (*pathLen)--;  // Backtrack
    return false;
}
```

### Diameter of Tree
```c
int diameterUtil(TreeNode* root, int* height) {
    if (root == NULL) {
        *height = 0;
        return 0;
    }
    
    int leftHeight = 0, rightHeight = 0;
    int leftDiameter = diameterUtil(root->left, &leftHeight);
    int rightDiameter = diameterUtil(root->right, &rightHeight);
    
    *height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    
    int throughRoot = leftHeight + rightHeight;
    int maxSubtree = leftDiameter > rightDiameter ? leftDiameter : rightDiameter;
    
    return throughRoot > maxSubtree ? throughRoot : maxSubtree;
}

int diameter(TreeNode* root) {
    int height = 0;
    return diameterUtil(root, &height);
}
```

### Mirror/Invert Tree
```c
void mirror(TreeNode* root) {
    if (root == NULL) return;
    
    // Swap children
    TreeNode* temp = root->left;
    root->left = root->right;
    root->right = temp;
    
    // Recurse
    mirror(root->left);
    mirror(root->right);
}
```

### Tree to Array (Serialization)
```c
void serialize(TreeNode* root, int arr[], int* index) {
    if (root == NULL) {
        arr[(*index)++] = -1;  // Marker for NULL
        return;
    }
    
    arr[(*index)++] = root->data;
    serialize(root->left, arr, index);
    serialize(root->right, arr, index);
}

TreeNode* deserialize(int arr[], int* index) {
    if (arr[*index] == -1) {
        (*index)++;
        return NULL;
    }
    
    TreeNode* root = createNode(arr[(*index)++]);
    root->left = deserialize(arr, index);
    root->right = deserialize(arr, index);
    
    return root;
}
```

## AVL Tree (Self-Balancing BST)

### AVL Node Structure
```c
typedef struct AVLNode {
    int data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

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
```

### AVL Rotations
```c
// Right rotation
AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    updateHeight(y);
    updateHeight(x);
    
    return x;
}

// Left rotation
AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    updateHeight(x);
    updateHeight(y);
    
    return y;
}
```

### AVL Insertion
```c
AVLNode* insertAVL(AVLNode* root, int data) {
    // Standard BST insertion
    if (root == NULL) {
        AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
        node->data = data;
        node->left = node->right = NULL;
        node->height = 1;
        return node;
    }
    
    if (data < root->data) {
        root->left = insertAVL(root->left, data);
    } else if (data > root->data) {
        root->right = insertAVL(root->right, data);
    } else {
        return root;  // Duplicate
    }
    
    // Update height
    updateHeight(root);
    
    // Get balance factor
    int balance = getBalance(root);
    
    // Left-Left case
    if (balance > 1 && data < root->left->data) {
        return rotateRight(root);
    }
    
    // Right-Right case
    if (balance < -1 && data > root->right->data) {
        return rotateLeft(root);
    }
    
    // Left-Right case
    if (balance > 1 && data > root->left->data) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    
    // Right-Left case
    if (balance < -1 && data < root->right->data) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    
    return root;
}
```

## Special Tree Types

### Complete Binary Tree Check
```c
bool isComplete(TreeNode* root) {
    if (root == NULL) return true;
    
    TreeNode* queue[100];
    int front = 0, rear = 0;
    bool nonFullNodeSeen = false;
    
    queue[rear++] = root;
    
    while (front < rear) {
        TreeNode* current = queue[front++];
        
        if (current->left) {
            if (nonFullNodeSeen) return false;
            queue[rear++] = current->left;
        } else {
            nonFullNodeSeen = true;
        }
        
        if (current->right) {
            if (nonFullNodeSeen) return false;
            queue[rear++] = current->right;
        } else {
            nonFullNodeSeen = true;
        }
    }
    
    return true;
}
```

### Full Binary Tree Check
```c
bool isFullBinaryTree(TreeNode* root) {
    if (root == NULL) return true;
    
    if (root->left == NULL && root->right == NULL) {
        return true;
    }
    
    if (root->left && root->right) {
        return isFullBinaryTree(root->left) && 
               isFullBinaryTree(root->right);
    }
    
    return false;
}
```

## Memory Management

### Delete Entire Tree
```c
void deleteTree(TreeNode** root) {
    if (*root == NULL) return;
    
    deleteTree(&((*root)->left));
    deleteTree(&((*root)->right));
    
    free(*root);
    *root = NULL;
}
```

## Time Complexity

| Operation | Average (Balanced) | Worst (Skewed) |
|-----------|-------------------|----------------|
| Search | O(log n) | O(n) |
| Insert | O(log n) | O(n) |
| Delete | O(log n) | O(n) |
| Traversal | O(n) | O(n) |
| Height | O(n) | O(n) |

## Space Complexity
- Tree storage: O(n)
- Recursive traversal: O(h) where h is height
- Iterative traversal: O(n) for level order, O(h) for others

## Common Applications
1. **BST**: Database indexing, priority queues
2. **AVL**: Where frequent lookups are needed
3. **Heap**: Priority queue implementation
4. **Trie**: Autocomplete, spell checkers
5. **B-Tree**: File systems, databases

## Quick Reference

| Tree Type | Property | Use Case |
|-----------|----------|----------|
| Binary Tree | Max 2 children | General hierarchical data |
| BST | Left < Root < Right | Searching, sorting |
| AVL | Height-balanced BST | Frequent searches |
| Complete | All levels full except last | Heaps |
| Full | 0 or 2 children | Expression trees |

## Exam Tips
1. Know all traversal methods (recursive and iterative)
2. Understand BST property for all operations
3. Practice tree construction from traversals
4. Remember height vs depth difference
5. Understand rotation operations for AVL
6. Know how to check tree properties
7. Practice with both recursive and iterative solutions
8. Always handle NULL cases in recursive functions