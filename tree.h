#ifndef TREE_H
#define TREE_H

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

struct WeightBalancedTree *newWeightBalancedTree();

typedef struct TreeNode TreeNode;

int searchOperation(TreeNode *root, int key);
int size(TreeNode *root);
int searchBinaryTree(TreeNode *root, int key);
void updatedSize(TreeNode *root);
void detectBinaryAnomaly(struct TreeNode *root, int threshold);
void detectAnomaliesOperation(struct TreeNode *root, int threshold, int *features, int numOfFeatures);
void insert(struct WeightBalancedTree *tree, int key, int anomaly);
void freeTree(struct TreeNode *root);
void detectAnomalies(struct WeightBalancedTree *tree, int threshold, int *features, int numOfFeatures);
void traversal(struct TreeNode *root);
TreeNode *insertBinaryTree(struct TreeNode *root, int key);
TreeNode *deleteByKey(struct TreeNode *root, int key);
TreeNode *createNode(int key); // for binary tree
TreeNode *rotateRight(struct TreeNode *root);
TreeNode *rotateLeft(struct TreeNode *root);
TreeNode *balancedSize(struct TreeNode *root);
TreeNode *insertKey(struct TreeNode *root, int key);
TreeNode *minValueNode(struct TreeNode *root);

#endif
