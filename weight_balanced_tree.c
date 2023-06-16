#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tree.h"
#include "utils.h"

#define DATASET 10000
#define KEYS 10000
#define VALUES 100

struct WeightBalancedTree *newWeightBalancedTree()
{
    struct WeightBalancedTree *tree = (struct WeightBalancedTree *)malloc(sizeof(struct WeightBalancedTree));
    if (tree == NULL)
    {
        return NULL;
    }

    tree->root = NULL;
    return tree;
}

struct TreeNode *createNode(int key)
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    if (node == NULL)
    {
        return NULL;
    }

    node->key = key;
    node->numOfAnomaly = 1; // for now just set with 1
    node->weight = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int searchOperation(struct TreeNode *root, int key)
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

int size(struct TreeNode *root)
{
    if (root == NULL)
    {
        return 0;
    }
    return root->weight;
}

void updatedSize(struct TreeNode *root)
{
    if (root == NULL)
    {
        printf("Tree is not initialized yet \n");
        return;
    }
    root->weight = 1 + size(root->left) + size(root->right);
}

struct TreeNode *rotateRight(struct TreeNode *root)
{
    struct TreeNode *newRoot = root->left;
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

struct TreeNode *rotateLeft(struct TreeNode *root)
{
    struct TreeNode *newRoot = root->right;
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

struct TreeNode *balancedSize(struct TreeNode *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    // float weightSize = root->left->weight / root->weight;

    if (size(root->left) < (2 * size(root->right)))
    {
        if (root->right != NULL && size(root->right->left) > size(root->right->right))
        {
            root->right = rotateRight(root->right);
        }
        root = rotateRight(root);
    }

    if (size(root->right) > (2 * size(root->left)))
    {
        if (root->left != NULL && size(root->left->right) > size(root->left->left))
        {
            root->left = rotateLeft(root->left);
        }
        root = rotateLeft(root);
    }

    return root;
}

struct TreeNode *insertKey(struct TreeNode *root, int key)
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

struct TreeNode *delete(struct TreeNode *root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (key < root->key)
    {
        root->left = delete (root->left, key);
    }
    else if (key > root->key)
    {
        root->right = delete (root->right, key);
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
            root->right = delete (root->right, key);
        }
        else
        {
            root = rotateLeft(root);
            root->left = delete (root->left, key);
        }
    }

    updatedSize(root);
    return balancedSize(root);
}

void insert(struct WeightBalancedTree *tree, int key, int anomaly)
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

void freeTree(struct TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void detectAnomaliesOperation(struct TreeNode *root, int threshold, int *features, int numOfFeatures)
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
void constantDetectAnomaly(struct TreeNode *root, int threshold)
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

void constantDetection(struct WeightBalancedTree *tree, int threshold)
{
    if (tree->root != NULL)
    {
        constantDetectAnomaly(tree->root, threshold);
    }
}

void detectAnomalies(struct WeightBalancedTree *tree, int threshold, int *features, int numOfFeatures)
{
    if (tree->root != NULL)
    {
        detectAnomaliesOperation(tree->root, threshold, features, numOfFeatures);
    }
}

void weightBalancedBinaryTreeTests()
{
    struct WeightBalancedTree *tree = newWeightBalancedTree();
    insert(tree, 1, 0);
    insert(tree, 3, 0);
    insert(tree, 4, 0);
    insert(tree, 6, 0);
    insert(tree, 2, 0);
    insert(tree, 11, 1);

    printf("=== Weighted Binary Tree Tests === \n");
    int searchKey = 1;
    int result = searchOperation(tree->root, searchKey);
    printf("Assertion for searching key %i %s", searchKey, result ? "Found" : "Not Found");

    freeTree(tree->root);
}

int main()
{

    struct BenchmarkResult benchmark;

    int dataset[DATASET][2];
    for (int i = 0; i < DATASET; i++)
    {
        dataset[i][0] = rand() % KEYS;
        // dataset[i][1] = rand() % VALUES;
    }

    struct WeightBalancedTree *tree = newWeightBalancedTree();

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
        delete (tree->root, deleteKey);
    }
    endBenchmark(&benchmark);
    double deleteTime = getBenchmarkResult(&benchmark);

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int insertKey = dataset[i][0];
        int maxTreshold = 10;
        int *features = &dataset[i][1];
        int numOfFeatures = sizeof(features) / sizeof(features[0]);
        detectAnomalies(tree, maxTreshold, features, numOfFeatures);
    }
    endBenchmark(&benchmark);
    double findingAnomalies = getBenchmarkResult(&benchmark);

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int insertKey = dataset[i][0];
        int maxTreshold = 10;
        constantDetection(tree, maxTreshold);
    }
    endBenchmark(&benchmark);
    double findingAnomaly = getBenchmarkResult(&benchmark);

    printf("Measuring insertion time: %f seconds \n", insertionTime);
    printf("Measuring searching time: %f seconds \n", searchingTime);
    printf("Measuring deletion time: %f seconds \n", deleteTime);
    printf("Measuring find anomaly time: %f seconds \n", findingAnomalies);
    printf("Measuring find anomaly time (constant): %f seconds \n", findingAnomaly);

    freeTree(tree->root);
    free(tree);

    return 0;
}
