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

WeightBalancedTree *new_weight_balanced_tree() {
    WeightBalancedTree *tree = (WeightBalancedTree *)malloc(sizeof(WeightBalancedTree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory location %s \n", strerror(errno));
        return NULL;
    }

    tree->root = NULL;
    return tree;
}

TreeNode *create_node(int key) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate memory location %s \n", strerror(errno));
        return NULL;
    }

    node->key = key;
    node->num_of_anomaly = 1;  // for now just set with 1
    node->weight = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int search_operation(TreeNode *root, int key) {
    if (root == NULL || root->key == key) {
        return root != NULL;
    }

    if (key > root->key) {
        return search_operation(root->left, key);
    } else {
        return search_operation(root->right, key);
    }
}

int size(TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    return root->weight;
}

void updated_size(TreeNode *root) {
    if (root == NULL) {
        printf("Tree is not initialized yet \n");
        return;
    }
    root->weight = 1 + size(root->left) + size(root->right);
}

TreeNode *rotate_right(TreeNode *root) {
    TreeNode *new_root = root->left;
    if (new_root == NULL) {
        return root;
    }
    root->left = new_root->right;
    new_root->right = root;
    updated_size(root);
    updated_size(new_root);
    return new_root;
}

TreeNode *rotate_left(TreeNode *root) {
    TreeNode *new_root = root->right;
    if (new_root == NULL) {
        return root;
    }
    root->right = new_root->left;
    new_root->left = root;
    updated_size(root);
    updated_size(new_root);
    return new_root;
}

TreeNode *balanced_size(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    // float weightSize = root->left->weight / root->weight;

    if (root->left != NULL && size(root->left) < (2 * size(root->right))) {
        if (root->right != NULL && size(root->right->left) > size(root->right->right)) {
            root->right = rotate_right(root->right);
        }
        root = rotate_right(root);
    }

    if (root->right != NULL && size(root->right) > (2 * size(root->left))) {
        if (root->left != NULL && size(root->left->right) > size(root->left->left)) {
            root->left = rotate_left(root->left);
        }
        root = rotate_left(root);
    }

    return root;
}

TreeNode *insert_key(TreeNode *root, int key) {
    if (root == NULL) {
        return create_node(key);
    }

    if (root->key == key) {
        root->num_of_anomaly += 1;
    } else if (key > root->key) {
        root->left = insert_key(root->left, key);
    } else if (key < root->key) {
        root->right = insert_key(root->right, key);
    }

    return balanced_size(root);
}

TreeNode *min_value_node(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    TreeNode *current = root;

    while (current && current->left != NULL) {
        current = current->left;
    }

    return current;
}

TreeNode *max_value_node(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    TreeNode *current = root;

    while (current && current->right != NULL) {
        current = current->right;
    }

    return current;
}

TreeNode *delete_by_key(TreeNode *root, int key) {
    if (root == NULL) {
        return NULL;
    }

    if (key < root->key) {
        root->left = delete_by_key(root->left, key);
    } else if (key > root->key) {
        root->right = delete_by_key(root->right, key);
    } else {
        if (root->left == NULL && root->right == NULL) {
            // handling when the root left and right if its null and just clean it up
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            TreeNode *temp_tree = root->right;
            free(root);
            return temp_tree;
        } else if (root->right == NULL) {
            TreeNode *temp_tree = root->left;
            free(root);
            return temp_tree;
        } else {
            // this should be fixed the key isn't deleted, the operation of deletion
            // should be deleted based on the minimum value and usually if we talked
            // about binary tree, the lowest value in tree it's come from right tree
            TreeNode *min_right_sub_tree = min_value_node(root->right);
            root->key = min_right_sub_tree->key;
            root->right = delete_by_key(root->right, min_right_sub_tree->key);
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
        tree->root = create_node(key);
    } else {
        tree->root = insert_key(tree->root, key);
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void free_weight_balanced_tree(WeightBalancedTree *tree) {
    // remove the memory allocation for weight balanced tree nodes
    // it's different with the remove the tree node
    if (tree == NULL) {
        return;
    }

    free_tree(tree->root);
    free(tree);
}

void detect_anomalies_operation(TreeNode *root, int threshold, int *features, int num_of_features, int *true_positive,
                                int *false_positive, int *false_negative) {
    if (root == NULL) {
        return;
    }

    if (root->weight > threshold) {
        int is_anomaly = 0;
        for (int i = 0; i < num_of_features; i++) {
            if (root->key == features[i]) {
                // printf("Anomaly is being detected with the weight of tree is %d\n", root->weight);
                is_anomaly = 1;
                break;
            }
        }

        if (is_anomaly == 0) {
            (*true_positive)++;
        } else {
            if (root->num_of_anomaly > 0) {
                (*false_negative)++;
            } else {
                (*false_positive)++;
            }
        }
    }

    detect_anomalies_operation(root->left, threshold, features, num_of_features, true_positive, false_positive,
                               false_positive);
    detect_anomalies_operation(root->right, threshold, features, num_of_features, true_positive, false_positive,
                               false_positive);
}

// instead using O(n) or linear time, how about if using constant time?
// so we just remove the features classification and only use max threshold
int constant_detect_anomaly(TreeNode *root, int threshold, int *true_positive, int *false_positive,
                            int *false_negative) {
    if (root == NULL) {
        return 0;
    }

    if (root->weight > threshold) {
        if (root->num_of_anomaly > 0) {
            (*true_positive)++;
            return -1;
        } else {
            (*false_positive)++;
        }
    } else {
        if (root->num_of_anomaly > 0) {
            (*false_negative)++;
        }
    }

    int find_right_anomaly =
        constant_detect_anomaly(root->right, threshold, true_positive, false_positive, false_negative);
    int find_left_anomaly =
        constant_detect_anomaly(root->left, threshold, true_positive, false_positive, false_negative);

    if (find_left_anomaly == -1 && find_right_anomaly == -1) {
        return -1;
    }

    return 0;
}

int constant_detection(WeightBalancedTree *tree, int threshold, int *true_positive, int *false_positive,
                       int *false_negative) {
    if (tree->root != NULL) {
        return constant_detect_anomaly(tree->root, threshold, true_positive, false_positive, false_negative);
    }

    return 0;
}

void detect_anomalies(WeightBalancedTree *tree, int threshold, int *features, int num_of_features, int *true_positive,
                      int *false_positive, int *false_negative) {
    if (tree->root != NULL) {
        detect_anomalies_operation(tree->root, threshold, features, num_of_features, true_positive, false_positive,
                                   false_negative);
    }
}

int main() {

    BenchmarkResult benchmark;

    int initial_true_positive = 0;
    int initial_false_positive = 0;
    int initial_false_negative = 0;
    int max_threshold = 0;
    int num_of_anomaly = 1;

    int dataset[DATASET][2];
    for (int i = 0; i < DATASET; i++) {
        dataset[i][0] = rand() % KEYS;
    }

    WeightBalancedTree *tree = new_weight_balanced_tree();

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int insert_key = dataset[i][0];
        insert(tree, insert_key, num_of_anomaly);
    }
    end_benchmark(&benchmark);
    double insertion_time = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int search_key = dataset[i][0];
        search_operation(tree->root, search_key);
    }
    end_benchmark(&benchmark);
    double searching_time = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int delete_key = dataset[i][0];
        delete_by_key(tree->root, delete_key);
    }
    end_benchmark(&benchmark);
    double delete_time = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int *features = dataset[i];
        size_t num_of_features = SIZEOF(dataset[i]);
        detect_anomalies(tree, max_threshold, features, num_of_features, &initial_true_positive,
                         &initial_false_positive, &initial_false_negative);
    }
    end_benchmark(&benchmark);
    double finding_anomalies = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        constant_detection(tree, max_threshold, &initial_true_positive, &initial_false_positive,
                           &initial_false_negative);
    }
    end_benchmark(&benchmark);
    double finding_anomaly = get_benchmark_result(&benchmark);

    weightBalancedBinaryTreeTests();

    printf("Measuring insertion time: %f seconds \n", insertion_time);
    printf("Measuring searching time: %f seconds \n", searching_time);
    printf("Measuring deletion time: %f seconds \n", delete_time);
    printf("Measuring find anomaly time: %f seconds \n", finding_anomalies);
    printf("Measuring find anomaly time (constant): %f seconds \n", finding_anomaly);

    free_weight_balanced_tree(tree);

    return 0;
}
