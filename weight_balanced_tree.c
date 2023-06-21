#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "tree.h"
#include "utils.h"

#define DATASET 10000
#define KEYS 10000
#define VALUES 100
#define SIZEOF(arr) sizeof(arr) / sizeof(*arr)

WeightBalancedTree *newWeightBalancedTree()
{
    WeightBalancedTree *tree = (WeightBalancedTree *)malloc(sizeof(WeightBalancedTree));
    if (tree == NULL)
    {
        fprintf(stderr, "Failed to allocate memory location %s \n", strerror(errno));
        return NULL;
    }

    tree->root = NULL;
    return tree;
}

TreeNode *createNode(int key)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory location %s \n", strerror(errno));
        return NULL;
    }

    node->key = key;
    node->numOfAnomaly = 1; // for now just set with 1
    node->weight = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int searchOperation(TreeNode *root, int key)
{
    if (root == NULL || root->key == key)
    {
        return root != NULL;
    }

    if (key > root->key)
    {
        return searchOperation(root->left, key);
    }
    else
    {
        return searchOperation(root->right, key);
    }
}

int size(TreeNode *root)
{
    if (root == NULL)
    {
        return 0;
    }
    return root->weight;
}

void updatedSize(TreeNode *root)
{
    if (root == NULL)
    {
        printf("Tree is not initialized yet \n");
        return;
    }
    root->weight = 1 + size(root->left) + size(root->right);
}

TreeNode *rotateRight(TreeNode *root)
{
    TreeNode *newRoot = root->left;
    if (newRoot == NULL)
    {
        return root;
    }
    root->left = newRoot->right;
    newRoot->right = root;
    updatedSize(root);
    updatedSize(newRoot);
    return newRoot;
}

TreeNode *rotateLeft(TreeNode *root)
{
    TreeNode *newRoot = root->right;
    if (newRoot == NULL)
    {
        return root;
    }
    root->right = newRoot->left;
    newRoot->left = root;
    updatedSize(root);
    updatedSize(newRoot);
    return newRoot;
}

TreeNode *balancedSize(TreeNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    // float weightSize = root->left->weight / root->weight;

    if (root->left != NULL && size(root->left) < (2 * size(root->right)))
    {
        if (root->right != NULL && size(root->right->left) > size(root->right->right))
        {
            root->right = rotateRight(root->right);
        }
        root = rotateRight(root);
    }

    if (root->right != NULL && size(root->right) > (2 * size(root->left)))
    {
        if (root->left != NULL && size(root->left->right) > size(root->left->left))
        {
            root->left = rotateLeft(root->left);
        }
        root = rotateLeft(root);
    }

    return root;
}

TreeNode *insertKey(TreeNode *root, int key)
{
    if (root == NULL)
    {
        return createNode(key);
    }

    if (root->key == key)
    {
        root->numOfAnomaly += 1;
    }
    else if (key > root->key)
    {
        root->left = insertKey(root->left, key);
    }
    else if (key < root->key)
    {
        root->right = insertKey(root->right, key);
    }

    return balancedSize(root);
}

TreeNode *deleteByKey(TreeNode *root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (key < root->key)
    {
        root->left = deleteByKey(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = deleteByKey(root->right, key);
    }
    else if (root->left == NULL)
    {
        return root->right;
    }
    else if (root->right == NULL)
    {
        return root->left;
    }
    else
    {
        if (size(root->left) > size(root->right))
        {
            root = rotateRight(root);
            root->right = deleteByKey(root->right, key);
        }
        else
        {
            root = rotateLeft(root);
            root->left = deleteByKey(root->left, key);
        }
    }

    updatedSize(root);
    return balancedSize(root);
}

void insert(WeightBalancedTree *tree, int key, __attribute__((unused)) int anomaly)
{
    if (tree == NULL)
    {
        printf("TreeNode is not initialized yet \n");
        return;
    }

    if (tree->root == NULL)
    {
        tree->root = createNode(key);
    }
    else
    {
        tree->root = insertKey(tree->root, key);
    }
}

void freeTree(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void freeWeightBalancedTree(WeightBalancedTree *tree)
{
    // remove the memory allocation for weight balanced tree nodes
    // it's different with the remove the tree node
    if (tree == NULL)
    {
        return;
    }

    freeTree(tree->root);
    free(tree);
}

void detectAnomaliesOperation(TreeNode *root, int threshold, int *features, int numOfFeatures)
{
    if (root == NULL)
    {
        return;
    }

    if (root->weight > threshold)
    {
        for (int i = 0; i < numOfFeatures; i++)
        {
            if (root->key == features[i])
            {
                printf("Anomaly is being detected with the weight of tree is %d\n", root->weight);
            }
        }
    }
}

// instead using O(n) or linear time, how about if using constant time?
// so we just remove the features classification and only use max threshold
void constantDetectAnomaly(TreeNode *root, int threshold)
{
    if (root != NULL)
    {
        return;
    }

    if (root->weight > threshold)
    {
        printf("Anomaly is being detected with the weight of tree is %d\n", root->weight);
    }

    constantDetectAnomaly(root->right, threshold);
    constantDetectAnomaly(root->left, threshold);
}

void constantDetection(WeightBalancedTree *tree, int threshold)
{
    if (tree->root != NULL)
    {
        constantDetectAnomaly(tree->root, threshold);
    }
}

void detectAnomalies(WeightBalancedTree *tree, int threshold, int *features, int numOfFeatures)
{
    if (tree->root != NULL)
    {
        detectAnomaliesOperation(tree->root, threshold, features, numOfFeatures);
    }
}

void weightBalancedBinaryTreeTests()
{
    WeightBalancedTree *tree = newWeightBalancedTree();
    TreeNode *root = NULL;
    insert(tree, 1, 0);
    insert(tree, 3, 0);
    insert(tree, 4, 0);
    insert(tree, 6, 0);
    insert(tree, 2, 0);
    insert(tree, 11, 1);

    printf("=== Weighted Binary Tree Tests === \n");
    int searchKey = 1;
    int result = searchOperation(tree->root, searchKey);
    printf("Assertion for searching key %i %s \n", searchKey, result ? "Is Found" : "Not Found");

    searchKey = 19;
    result = searchOperation(tree->root, searchKey);
    printf("Assertion for searching key (invalid key) %i %s \n", searchKey, result ? "Is Found" : "Not Found");

    int delKey = 11;
    WeightBalancedTree *deletion = (WeightBalancedTree *)deleteByKey(root, delKey);

    // this will lead into another segmentation fault errors
    // TODO: fix what causing this error before the deletion process (balancing function)
    printf("Assertion for deleting key %d \n", deletion->root->key);

    int maxThreshold = 7;
    constantDetectAnomaly(root, maxThreshold);
    printf("Detection anomaly with constant time \n");

    freeTree(tree->root);
}

int main()
{

    BenchmarkResult benchmark;

    int dataset[DATASET][2];
    for (int i = 0; i < DATASET; i++)
    {
        dataset[i][0] = rand() % KEYS;
        // dataset[i][1] = rand() % VALUES;
    }

    WeightBalancedTree *tree = newWeightBalancedTree();

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int insertKey = dataset[i][0];
        // int insertValue = dataset[i][1];
        int numOfAnomaly = 1;
        insert(tree, insertKey, numOfAnomaly);
    }
    endBenchmark(&benchmark);
    double insertionTime = getBenchmarkResult(&benchmark);

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int searchKey = dataset[i][0];
        searchOperation(tree->root, searchKey);
    }
    endBenchmark(&benchmark);
    double searchingTime = getBenchmarkResult(&benchmark);

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int deleteKey = dataset[i][0];
        deleteByKey(tree->root, deleteKey);
    }
    endBenchmark(&benchmark);
    double deleteTime = getBenchmarkResult(&benchmark);

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int maxTreshold = 10;
        int *features = dataset[i];
        size_t numOfFeatures = SIZEOF(dataset[i]);
        detectAnomalies(tree, maxTreshold, features, numOfFeatures);
    }
    endBenchmark(&benchmark);
    double findingAnomalies = getBenchmarkResult(&benchmark);

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int maxTreshold = 10;
        constantDetection(tree, maxTreshold);
    }
    endBenchmark(&benchmark);
    double findingAnomaly = getBenchmarkResult(&benchmark);

    weightBalancedBinaryTreeTests();

    printf("Measuring insertion time: %f seconds \n", insertionTime);
    printf("Measuring searching time: %f seconds \n", searchingTime);
    printf("Measuring deletion time: %f seconds \n", deleteTime);
    printf("Measuring find anomaly time: %f seconds \n", findingAnomalies);
    printf("Measuring find anomaly time (constant): %f seconds \n", findingAnomaly);

    freeWeightBalancedTree(tree);

    return 0;
}
