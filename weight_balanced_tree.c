#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATASET 10000
#define KEYS 10000
#define VALUES 100

struct TreeNode
{
    int key;
    int weight;
    int numOfAnomaly;
    struct TreeNode *right;
    struct TreeNode *left;
};

struct WeightBalancedTree
{
    struct TreeNode *root;
};

struct WeightBalancedTree *newWeightBalancedTree()
{
    struct WeightBalancedTree *tree = (struct WeightBalancedTree *)malloc(sizeof(struct WeightBalancedTree));
    tree->root = NULL;
    return tree;
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
    root->weight = 1 + size(root->left) + size(root->right);
}

struct TreeNode *rotateRight(struct TreeNode *root)
{
    struct TreeNode *newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;
    updatedSize(root);
    updatedSize(root->right);
    return newRoot;
}

struct TreeNode *rotateLeft(struct TreeNode *root)
{
    struct TreeNode *newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;
    updatedSize(root);
    updatedSize(root->left);
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
        if (size(root->right->left) > size(root->right->right))
        {
            root->right = rotateRight(root->right);
        }
        root = rotateRight(root);
    }

    if (size(root->right) > (2 * size(root->left)))
    {
        if (size(root->left->right) > size(root->left->left))
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
        struct TreeNode *newNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));
        newNode->key = key;
        newNode->numOfAnomaly = 1;
        return newNode;
    }

    if (root->key == key)
    {
        root->numOfAnomaly += 1;
    }
    else if (key > root->key)
    {
        root->left = insertKey(root->left, key);
    }
    else
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
    if (tree->root == NULL)
    {
        tree->root = (struct TreeNode *)malloc(sizeof(struct TreeNode));
        tree->root->key = key;
        tree->root->numOfAnomaly = anomaly;
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
        return constantDetectAnomaly(tree->root, threshold);
    }
}

void detectAnomalies(struct WeightBalancedTree *tree, int threshold, int *features, int numOfFeatures)
{
    if (tree->root != NULL)
    {
        detectAnomaliesOperation(tree->root, threshold, features, numOfFeatures);
    }
}

int main()
{

    srand(time(NULL));

    int dataset[DATASET][2];
    for (int i = 0; i < DATASET; i++)
    {
        dataset[i][0] = rand() % KEYS;
        // dataset[i][1] = rand() % VALUES;
    }

    struct WeightBalancedTree *tree = newWeightBalancedTree();

    clock_t startTime = clock();
    for (int i = 0; i < DATASET; i++)
    {
        int insertKey = dataset[i][0];
        // int insertValue = dataset[i][1];
        int numOfAnomaly = 1;
        insert(tree, insertKey, numOfAnomaly);
    }
    double insertionTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

    startTime = clock();
    for (int i = 0; i < DATASET; i++)
    {
        int searchKey = dataset[i][0];
        searchOperation(tree->root, searchKey);
    }
    double searchingTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

    startTime = clock();
    for (int i = 0; i < DATASET; i++)
    {
        int deleteKey = dataset[i][0];
        delete (tree->root, deleteKey);
    }
    double deleteTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

    startTime = clock();
    for (int i = 0; i < DATASET; i++)
    {
        int insertKey = dataset[i][0];
        int maxTreshold = 10;
        int *features = &dataset[i][1];
        int numOfFeatures = 1;
        detectAnomalies(tree, maxTreshold, features, numOfFeatures);
    }
    double findingAnomalies = (double)(clock() - startTime) / CLOCKS_PER_SEC;

    startTime = clock();
    for (int i = 0; i < DATASET; i++)
    {
        int insertKey = dataset[i][0];
        int maxTreshold = 10;
        constantDetection(tree, maxTreshold);
    }
    double findingAnomaly = (double)(clock() - startTime) / CLOCKS_PER_SEC;

    printf("Measuring insertion time: %f seconds \n", insertionTime);
    printf("Measuring searching time: %f seconds \n", searchingTime);
    printf("Measuring deletion time: %f seconds \n", deleteTime);
    printf("Measuring find anomaly time: %f seconds \n", findingAnomalies);
    printf("Measuring find anomaly time (constant): %f seconds \n", findingAnomaly);

    freeTree(tree->root);
    free(tree);

    return 0;
}
