#ifndef TREE_H
#define TREE_H

typedef struct TreeNode TreeNode;
struct TreeNode {
    int key;
    int weight;
    int num_of_anomaly;
    struct TreeNode *right;
    struct TreeNode *left;
};

typedef struct {
    TreeNode *root;
} WeightBalancedTree;

WeightBalancedTree *new_weight_balanced_tree();

int search_operation(TreeNode *root, int key);
int size(TreeNode *root);
int search_binary_tree(TreeNode *root, int key);
int constant_detect_anomaly(TreeNode *root, int threshold, int *true_positive, int *false_positive,
                            int *false_negative);
int constant_detection(WeightBalancedTree *tree, int threshold, int *true_positive, int *false_positive,
                       int *false_negative);
int detect_binary_anomaly(TreeNode *root, int threshold, int *true_positive, int *false_positive, int *false_negative);
void updated_size(TreeNode *root);
void detect_anomalies_operation(TreeNode *root, int threshold, int *features, int num_of_features, int *true_positive,
                                int *false_positive, int *false_negative);
void insert(WeightBalancedTree *tree, int key, int anomaly);
void free_tree(TreeNode *root);
void detect_anomalies(WeightBalancedTree *tree, int threshold, int *features, int num_of_features, int *true_positive,
                      int *false_positive, int *false_negative);
void traversal(TreeNode *root);
TreeNode *insert_binary_tree(TreeNode *root, int key);
TreeNode *delete_by_key(TreeNode *root, int key);
TreeNode *create_node(int key);  // for binary tree
TreeNode *rotate_right(TreeNode *root);
TreeNode *rotate_left(TreeNode *root);
TreeNode *balanced_size(TreeNode *root);
TreeNode *insert_key(TreeNode *root, int key);
TreeNode *min_value_node(TreeNode *root);
TreeNode *max_value_node(TreeNode *root);

#endif
