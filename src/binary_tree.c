// #include <curl/curl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/tree.h"
#include "../include/utils.h"
#include "../tests/tests_binary_tree.c"

#define DATASET 10000
#define KEYS 10000

int search_binary_tree(TreeNode *root, int key) {
    if (root == NULL) {
        return 0;
    }

    if (root->key == key) {
        return 1;
    }

    if (key < root->key) {
        return search_binary_tree(root->left, key);
    } else {
        return search_binary_tree(root->right, key);
    }
}

TreeNode *create_node(int key) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate the memory into tree node %s \n", strerror(errno));
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->weight = 1;
    return node;
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

// main source: https://www.techiedelight.com/deletion-from-bst/
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
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            TreeNode *temp_root = root->right;
            free(root);
            return temp_root;
        } else if (root->right == NULL) {
            TreeNode *temp_root = root->left;
            free(root);
            return temp_root;
        } else {
            TreeNode *min_right_sub_tree = min_value_node(root->right);
            root->key = min_right_sub_tree->key;
            root->right = delete_by_key(root->right, min_right_sub_tree->key);
        }
    }

    return root;
}

TreeNode *insert_binary_tree(TreeNode *root, int key) {
    if (root == NULL) {
        return create_node(key);
    }

    if (key < root->key) {
        root->left = insert_binary_tree(root->left, key);
    } else if (key > root->key) {
        root->right = insert_binary_tree(root->right, key);
    } else {
        root->weight += 1;
    }

    return root;
}

int detect_binary_anomaly(TreeNode *root, int threshold, int *true_positive, int *false_positive, int *false_negative) {
    if (root == NULL) {
        return 0;
    }

    if (root->weight > threshold) {
        if (root->num_of_anomaly > 0) {
            (*true_positive)++;
            return 1;
        } else {
            (*false_positive)++;
        }
    } else {
        (*false_negative)++;
    }

    int find_left_anomaly = detect_binary_anomaly(root->left, threshold, true_positive, false_positive, false_negative);
    int find_right_anomaly =
        detect_binary_anomaly(root->right, threshold, true_positive, false_positive, false_negative);

    if (find_left_anomaly == 1 && find_right_anomaly == 1) {
        return 1;
    }

    return 0;
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void traversal(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    traversal(root->right);
    printf("Traversal Binary Tree: %i", root->key);
    traversal(root->left);
}

int main() {

    BenchmarkResult benchmark;
    TreeNode *tree = create_node(5);

    insert_binary_tree(tree, 3);
    insert_binary_tree(tree, 5);
    insert_binary_tree(tree, 4);
    insert_binary_tree(tree, 8);
    insert_binary_tree(tree, 2);
    insert_binary_tree(tree, 6);

    start_benchmark(&benchmark);
    min_value_node(tree);
    end_benchmark(&benchmark);
    double find_min_value = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    max_value_node(tree);
    end_benchmark(&benchmark);
    double find_max_value = get_benchmark_result(&benchmark);

    int dataset[DATASET][2];
    for (int i = 0; i < DATASET; i++) {
        dataset[i][0] = rand() % KEYS;
    }

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int insert_key = dataset[i][0];
        create_node(insert_key);
    }
    end_benchmark(&benchmark);
    double insertion_time = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int search_key = dataset[i][0];
        search_binary_tree(tree, search_key);
    }
    end_benchmark(&benchmark);
    double search_time = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int delete_key = dataset[i][0];
        delete_by_key(tree, delete_key);
    }
    end_benchmark(&benchmark);
    double delete_time = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int insert_key = dataset[i][0];
        create_node(insert_key);
        int max_threshold = 0;
        int initial_true_positive = 0;
        int initial_false_negative = 0;
        int initial_false_positive = 0;
        detect_binary_anomaly(tree, max_threshold, &initial_true_positive, &initial_false_positive,
                              &initial_false_negative);
    }
    end_benchmark(&benchmark);
    double finding_anomalies = get_benchmark_result(&benchmark);

    // Implementation of Anomaly detection using HTTP request
    // TODO: fix this request, since it will got an error from response
    // curl_global_init(CURL_GLOBAL_DEFAULT);

    // request = curl_easy_init();
    // if (request)
    // {
    //     curl_easy_setopt(request, CURLOPT_URL, "https://jsonplaceholder.typicode.com/1");
    //     response = curl_easy_perform(request);
    //     if (response != CURLE_OK)
    //     {
    //         long responseCode;
    //         curl_easy_getinfo(request, CURLINFO_RESPONSE_CODE, &responseCode);
    //         printf("Failed when making HTTP request \n");
    //         printf("Response status code is: %ld \n", responseCode);
    //     }

    //     int maxThreshold = 1;
    //     detectBinaryAnomaly(tree, maxThreshold);
    //     freeTree(tree->left);
    //     freeTree(tree->right);
    //     free(tree);

    //     curl_easy_cleanup(request);
    // }

    // curl_global_cleanup();

    binaryTreeTests();

    printf("Measuring finding minimum key: %f seconds \n", find_min_value);
    printf("Measuring finding maximum key: %f seconds \n", find_max_value);
    printf("Measuring insertion time: %f seconds \n", insertion_time);
    printf("Measuring searching time: %f seconds \n", search_time);
    printf("Measuring deletion time %f seconds \n", delete_time);
    printf("Measuring find anomalies: %f seconds \n", finding_anomalies);

    free_tree(tree);

    return 0;
}
