#include <stdio.h>
#include <stdlib.h>

typedef struct Tree {
    struct Node *root;
} Tree;

typedef struct Node {
    int x;
    int y;
    int sum;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

void freeAll(Node *rootNode) {
    if (rootNode) {
        freeAll(rootNode->left);
	freeAll(rootNode->right);
        free(rootNode);
    }
}

void freeMethod(Node **rootNode) {
    freeAll(*rootNode);
    *rootNode = NULL;
}

Tree *createTree() {
    Tree *tree = malloc(sizeof(Tree));
    if(tree) {
        tree->root = NULL;
    }
    return tree;
}
 
Node *createNewNode(int x, int y) {
    Node *newNode = malloc(sizeof(Node));
    if(newNode) {
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->x = x;
        newNode->y = y;
    }
    return newNode;
}

Node *LocateParent(Tree *tree, Node *nodeToInsert) {
    Node *futureParent = NULL;
    Node *root = tree->root;
    
    while(root) {
        futureParent = root;
        if (nodeToInsert->x < root->x) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    
    return futureParent;
}

void TreeInsert(Tree *tree, Node *nodeToInsert) {
    Node *futureParent = LocateParent(tree, nodeToInsert);
    nodeToInsert->parent = futureParent;
    if(futureParent == NULL) {
        tree->root = nodeToInsert;
    } else if (nodeToInsert->x < futureParent->x) {
        futureParent->left = nodeToInsert;
    } else {
        futureParent->right = nodeToInsert;
    }
}

void InsertAndUpdateSum(Tree *tree, Node *nodeToInsert) {
    TreeInsert(tree, nodeToInsert);
    nodeToInsert->sum = nodeToInsert->y;
    Node *parentNode = nodeToInsert->parent;
    while(parentNode) {
        parentNode->sum += nodeToInsert->y;
        parentNode = parentNode->parent;
    }
}

int TreeSumGe(Node *node, int K) {
    int sum = 0;
    Node *traversalNode = node;
    while (traversalNode) {
        if (traversalNode->x >= K) {
            sum += traversalNode->y;
            if (traversalNode->right) {
                sum += traversalNode->right->sum;
            }
            traversalNode = traversalNode->left;
        } else {
            traversalNode = traversalNode->right;
        }
    }
    return sum;
}

int TreeSumLe(Node *node, int K) {
    int sum = 0;
    Node *traversalNode = node;
    while (traversalNode) {
        if (traversalNode->x <= K) {
            sum += traversalNode->y;
            if (traversalNode->left) {
                sum += traversalNode->left->sum;
            }
            traversalNode = traversalNode->right;
        } else {
            traversalNode = traversalNode->left;
        }
    }
    
    return sum;
}

int sumYinRangeX(Node *node, int min, int max) {
    Node *traversalNode = node;
    int leftSum = 0;
    int rightSum = 0;
    while (traversalNode && ((traversalNode->x >= min && traversalNode->x <= max) == 0)) {
        if (traversalNode->x <= min) {
            traversalNode = traversalNode->right;
        } else if (traversalNode->x >= max) {
            traversalNode = traversalNode->left;
        }
    }
    
    if (traversalNode) {
        leftSum = TreeSumGe(traversalNode->left, min);
        rightSum = TreeSumLe(traversalNode->right, max);
        return traversalNode->y + leftSum + rightSum;
    } else {
        return 0;
    }
}

int main(int argc, char *argv[]) {
    int x;
    int y;
    int min;
    int max;
    int sum;
    Tree *tree = createTree();
    if (argc > 1) {
        FILE *dataFile  = fopen(argv[1], "r");
        if (dataFile) {
            while (fscanf(dataFile, " %d %d", &x, &y) != EOF) {
	    	InsertAndUpdateSum(tree, createNewNode(x, y));
	    }
        } else {
            printf("Data file does not exist.\n");
        }
        FILE *rangeFile = fopen(argv[2], "r");
        if(rangeFile) {
            while (fscanf(rangeFile, "%d %d", &min, &max) != EOF) {
                sum = sumYinRangeX(tree->root, min, max);
                printf("%d %d %d\n", min, max, sum);
            }
        } else {
            printf("Range file does not exist\n");
        }
    }
   
    freeMethod(&(tree->root));
    free(tree);
    tree = NULL; 
    return 0;
}

