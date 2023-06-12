#include <stdio.h>
#include <stdlib.h>

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

void detectAnomalies(struct WeightBalancedTree *tree, int threshold, int *features, int numOfFeatures)
{
    if (tree->root != NULL)
    {
        detectAnomaliesOperation(tree->root, threshold, features, numOfFeatures);
    }
}
