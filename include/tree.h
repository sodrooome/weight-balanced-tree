#ifndef TREE_H
#define TREE_H

typedef struct TreeNode TreeNode;
struct TreeNode {
    int key;
    int weight;
    int numOfAnomaly;
    struct TreeNode *right;
    struct TreeNode *left;
};

typedef struct {
    TreeNode *root;
} WeightBalancedTree;

WeightBalancedTree *newWeightBalancedTree();

int searchOperation(TreeNode *root, int key);
int size(TreeNode *root);
int searchBinaryTree(TreeNode *root, int key);
void updatedSize(TreeNode *root);
void detectBinaryAnomaly(TreeNode *root, int threshold);
void detectAnomaliesOperation(TreeNode *root, int threshold, int *features, int numOfFeatures);
void insert(WeightBalancedTree *tree, int key, int anomaly);
void freeTree(TreeNode *root);
void detectAnomalies(WeightBalancedTree *tree, int threshold, int *features, int numOfFeatures);
void constantDetectAnomaly(TreeNode *root, int threshold);
void constantDetection(WeightBalancedTree *tree, int threshold);
void traversal(TreeNode *root);
TreeNode *insertBinaryTree(TreeNode *root, int key);
TreeNode *deleteByKey(TreeNode *root, int key);
TreeNode *createNode(int key);  // for binary tree
TreeNode *rotateRight(TreeNode *root);
TreeNode *rotateLeft(TreeNode *root);
TreeNode *balancedSize(TreeNode *root);
TreeNode *insertKey(TreeNode *root, int key);
TreeNode *minValueNode(TreeNode *root);
TreeNode *maxValueNode(TreeNode *root);

#endif
