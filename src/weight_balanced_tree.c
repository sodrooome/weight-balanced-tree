#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/tree.h"
#include "../include/utils.h"
#include "../tests/tests_weighted_binary_tree.c"

#define DATASET 10000
#define KEYS 10000
#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)

WeightBalancedTree *newWeightBalancedTree() {
    WeightBalancedTree *tree = (WeightBalancedTree *)malloc(sizeof(WeightBalancedTree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory location %s \n", strerror(errno));
        return NULL;
    }

    tree->root = NULL;
    return tree;
}

TreeNode *createNode(int key) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate memory location %s \n", strerror(errno));
        return NULL;
    }

    node->key = key;
    node->numOfAnomaly = 1;  // for now just set with 1
    node->weight = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int searchOperation(TreeNode *root, int key) {
    if (root == NULL || root->key == key) {
        return root != NULL;
    }

    if (key > root->key) {
        return searchOperation(root->left, key);
    } else {
        return searchOperation(root->right, key);
    }
}

int size(TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    return root->weight;
}

void updatedSize(TreeNode *root) {
    if (root == NULL) {
        printf("Tree is not initialized yet \n");
        return;
    }
    root->weight = 1 + size(root->left) + size(root->right);
}

TreeNode *rotateRight(TreeNode *root) {
    TreeNode *newRoot = root->left;
    if (newRoot == NULL) {
        return root;
    }
    root->left = newRoot->right;
    newRoot->right = root;
    updatedSize(root);
    updatedSize(newRoot);
    return newRoot;
}

TreeNode *rotateLeft(TreeNode *root) {
    TreeNode *newRoot = root->right;
    if (newRoot == NULL) {
        return root;
    }
    root->right = newRoot->left;
    newRoot->left = root;
    updatedSize(root);
    updatedSize(newRoot);
    return newRoot;
}

TreeNode *balancedSize(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    // float weightSize = root->left->weight / root->weight;

    if (root->left != NULL && size(root->left) < (2 * size(root->right))) {
        if (root->right != NULL && size(root->right->left) > size(root->right->right)) {
            root->right = rotateRight(root->right);
        }
        root = rotateRight(root);
    }

    if (root->right != NULL && size(root->right) > (2 * size(root->left))) {
        if (root->left != NULL && size(root->left->right) > size(root->left->left)) {
            root->left = rotateLeft(root->left);
        }
        root = rotateLeft(root);
    }

    return root;
}

TreeNode *insertKey(TreeNode *root, int key) {
    if (root == NULL) {
        return createNode(key);
    }

    if (root->key == key) {
        root->numOfAnomaly += 1;
    } else if (key > root->key) {
        root->left = insertKey(root->left, key);
    } else if (key < root->key) {
        root->right = insertKey(root->right, key);
    }

    return balancedSize(root);
}

TreeNode *minValueNode(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    TreeNode *current = root;

    while (current && current->left != NULL) {
        current = current->left;
    }

    return current;
}

TreeNode *maxValueNode(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    TreeNode *current = root;

    while (current && current->right != NULL) {
        current = current->right;
    }

    return current;
}

TreeNode *deleteByKey(TreeNode *root, int key) {
    if (root == NULL) {
        return NULL;
    }

    if (key < root->key) {
        root->left = deleteByKey(root->left, key);
    } else if (key > root->key) {
        root->right = deleteByKey(root->right, key);
    } else {
        if (root->left == NULL && root->right == NULL) {
            // handling when the root left and right if its null and just clean it up
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            TreeNode *tempTree = root->right;
            free(root);
            return tempTree;
        } else if (root->right == NULL) {
            TreeNode *tempTree = root->left;
            free(root);
            return tempTree;
        } else {
            // this should be fixed the key isn't deleted, the operation of deletion
            // should be deleted based on the minimum value and usually if we talked
            // about binary tree, the lowest value in tree it's come from right tree
            TreeNode *minRightSubtree = minValueNode(root->right);
            root->key = minRightSubtree->key;
            root->right = deleteByKey(root->right, minRightSubtree->key);
        }
    }

    return root;
}

void insert(WeightBalancedTree *tree, int key, __attribute__((unused)) int anomaly) {
    if (tree == NULL) {
        printf("TreeNode is not initialized yet \n");
        return;
    }

    if (tree->root == NULL) {
        tree->root = createNode(key);
    } else {
        tree->root = insertKey(tree->root, key);
    }
}

void freeTree(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void freeWeightBalancedTree(WeightBalancedTree *tree) {
    // remove the memory allocation for weight balanced tree nodes
    // it's different with the remove the tree node
    if (tree == NULL) {
        return;
    }

    freeTree(tree->root);
    free(tree);
}

void detectAnomaliesOperation(TreeNode *root, int threshold, int *features, int numOfFeatures, int *truePositive,
                              int *falsePositive, int *falseNegative) {
    if (root == NULL) {
        return;
    }

    if (root->weight > threshold) {
        int isAnomaly = 0;
        for (int i = 0; i < numOfFeatures; i++) {
            if (root->key == features[i]) {
                // printf("Anomaly is being detected with the weight of tree is %d\n", root->weight);
                isAnomaly = 1;
                break;
            }
        }

        if (isAnomaly == 0) {
            (*truePositive)++;
        } else {
            if (root->numOfAnomaly > 0) {
                (*falseNegative)++;
            } else {
                (*falsePositive)++;
            }
        }
    }

    detectAnomaliesOperation(root->left, threshold, features, numOfFeatures, truePositive, falsePositive,
                             falseNegative);
    detectAnomaliesOperation(root->right, threshold, features, numOfFeatures, truePositive, falsePositive,
                             falseNegative);
}

// instead using O(n) or linear time, how about if using constant time?
// so we just remove the features classification and only use max threshold
int constantDetectAnomaly(TreeNode *root, int threshold, int *truePositive, int *falsePositive, int *falseNegative) {
    if (root == NULL) {
        return 0;
    }

    if (root->weight > threshold) {
        if (root->numOfAnomaly > 0) {
            (*truePositive)++;
            return -1;
        } else {
            (*falsePositive)++;
        }
    } else {
        if (root->numOfAnomaly > 0) {
            (*falseNegative)++;
        }
    }

    int findRightAnomaly = constantDetectAnomaly(root->right, threshold, truePositive, falsePositive, falseNegative);
    int findLeftAnomaly = constantDetectAnomaly(root->left, threshold, truePositive, falsePositive, falseNegative);

    if (findLeftAnomaly == -1 && findRightAnomaly == -1) {
        return -1;
    }

    return 0;
}

int constantDetection(WeightBalancedTree *tree, int threshold, int *truePositive, int *falsePositive,
                      int *falseNegative) {
    if (tree->root != NULL) {
        return constantDetectAnomaly(tree->root, threshold, truePositive, falsePositive, falseNegative);
    }

    return 0;
}

void detectAnomalies(WeightBalancedTree *tree, int threshold, int *features, int numOfFeatures, int *truePositive,
                     int *falsePositive, int *falseNegative) {
    if (tree->root != NULL) {
        detectAnomaliesOperation(tree->root, threshold, features, numOfFeatures, truePositive, falsePositive,
                                 falseNegative);
    }
}

int main() {

    BenchmarkResult benchmark;

    int dataset[DATASET][2];
    for (int i = 0; i < DATASET; i++) {
        dataset[i][0] = rand() % KEYS;
    }

    WeightBalancedTree *tree = newWeightBalancedTree();

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int insertKey = dataset[i][0];
        int numOfAnomaly = 1;
        insert(tree, insertKey, numOfAnomaly);
    }
    end_benchmark(&benchmark);
    double insertionTime = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int searchKey = dataset[i][0];
        searchOperation(tree->root, searchKey);
    }
    end_benchmark(&benchmark);
    double searchingTime = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int deleteKey = dataset[i][0];
        deleteByKey(tree->root, deleteKey);
    }
    end_benchmark(&benchmark);
    double deleteTime = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int maxTreshold = 0;
        int *features = dataset[i];
        size_t numOfFeatures = SIZEOF(dataset[i]);
        int initialTruePositive = 0;
        int initialFalsePositive = 0;
        int initialFalseNegative = 0;
        detectAnomalies(tree, maxTreshold, features, numOfFeatures, &initialTruePositive, &initialFalsePositive,
                        &initialFalseNegative);
    }
    end_benchmark(&benchmark);
    double findingAnomalies = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int maxTreshold = 0;
        int initialTruePositive = 0;
        int initialFalsePositive = 0;
        int initialFalseNegative = 0;
        constantDetection(tree, maxTreshold, &initialTruePositive, &initialFalsePositive, &initialFalseNegative);
    }
    end_benchmark(&benchmark);
    double findingAnomaly = get_benchmark_result(&benchmark);

    weightBalancedBinaryTreeTests();

    printf("Measuring insertion time: %f seconds \n", insertionTime);
    printf("Measuring searching time: %f seconds \n", searchingTime);
    printf("Measuring deletion time: %f seconds \n", deleteTime);
    printf("Measuring find anomaly time: %f seconds \n", findingAnomalies);
    printf("Measuring find anomaly time (constant): %f seconds \n", findingAnomaly);

    freeWeightBalancedTree(tree);

    return 0;
}
