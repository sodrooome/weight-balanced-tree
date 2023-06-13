#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATASET 10000
#define KEYS 10000

struct TreeNode
{
    int key;
    struct TreeNode *right;
    struct TreeNode *left;
};

int searchBinaryTree(struct TreeNode *root, int key)
{
    if (root == NULL || root->key == key)
    {
        return root != NULL;
    }

    if (key < root->key)
    {
        return searchBinaryTree(root->left, key);
    }
    else
    {
        return searchBinaryTree(root->right, key);
    }
}

struct TreeNode *createNode(int key)
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void insertBinaryTree(struct TreeNode *root, int key)
{
    if (root == NULL)
    {
        return;
    }

    if (key < root->key)
    {
        return insertBinaryTree(root->left, key);
    }
    else
    {
        return insertBinaryTree(root->right, key);
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

int main()
{

    srand(time(NULL));

    int dataset[DATASET][2];
    for (int i = 0; i < DATASET; i++)
    {
        dataset[i][0] = rand() % KEYS;
    }

    struct TreeNode *tree;

    clock_t startTime = clock();
    for (int i = 0; i < DATASET; i++)
    {
        int insertKey = dataset[i][0];
        createNode(insertKey);
    }
    double insertionTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

    startTime = clock();
    for (int i = 0; i < DATASET; i++) {
        int searchKey = dataset[i][0];
        searchBinaryTree(tree, searchKey);
    }
    double searchTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

    printf("Measuring insertion time: %f seconds \n", insertionTime);
    printf("Measuring searching time: %f seconds \n", searchTime);

    freeTree(tree->left);
    freeTree(tree->right);
    free(tree);
}
