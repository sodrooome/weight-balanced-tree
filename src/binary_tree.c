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

int searchBinaryTree(TreeNode *root, int key) {
    if (root == NULL) {
        return 0;
    }

    if (root->key == key) {
        return 1;
    }

    if (key < root->key) {
        return searchBinaryTree(root->left, key);
    } else {
        return searchBinaryTree(root->right, key);
    }
}

TreeNode *createNode(int key) {
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

TreeNode *minValueNode(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    TreeNode *current = root;

    while (current && current->left != NULL) {
        current = current->left;
    }

    return current;
}

TreeNode *maxValueNode(TreeNode *root) {
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
TreeNode *deleteByKey(TreeNode *root, int key) {
    if (root == NULL) {
        return NULL;
    }

    if (key < root->key) {
        root->left = deleteByKey(root->left, key);
    } else if (key > root->key) {
        root->right = deleteByKey(root->right, key);
    } else {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            TreeNode *tempRoot = root->right;
            free(root);
            return tempRoot;
        } else if (root->right == NULL) {
            TreeNode *tempRoot = root->left;
            free(root);
            return tempRoot;
        } else {
            TreeNode *minRightSubtree = minValueNode(root->right);
            root->key = minRightSubtree->key;
            root->right = deleteByKey(root->right, minRightSubtree->key);
        }
    }

    return root;
}

TreeNode *insertBinaryTree(TreeNode *root, int key) {
    if (root == NULL) {
        return createNode(key);
    }

    if (key < root->key) {
        root->left = insertBinaryTree(root->left, key);
    } else if (key > root->key) {
        root->right = insertBinaryTree(root->right, key);
    } else {
        root->weight += 1;
    }

    return root;
}

int detectBinaryAnomaly(TreeNode *root, int threshold, int *truePositive, int *falsePositive, int *falseNegative) {
    if (root == NULL) {
        return 0;
    }

    if (root->weight > threshold) {
        if (root->numOfAnomaly > 0) {
            (*truePositive)++;
            return 1;
        } else {
            (*falsePositive)++;
        }
    } else {
        (*falseNegative)++;
    }

    int findLeftAnomaly = detectBinaryAnomaly(root->left, threshold, truePositive, falsePositive, falseNegative);
    int findRightAnomaly = detectBinaryAnomaly(root->right, threshold, truePositive, falsePositive, falseNegative);

    if (findLeftAnomaly == 1 && findRightAnomaly == 1) {
        return 1;
    }

    return 0;
}

void freeTree(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
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
    TreeNode *tree = createNode(5);

    insertBinaryTree(tree, 3);
    insertBinaryTree(tree, 5);
    insertBinaryTree(tree, 4);
    insertBinaryTree(tree, 8);
    insertBinaryTree(tree, 2);
    insertBinaryTree(tree, 6);

    start_benchmark(&benchmark);
    minValueNode(tree);
    end_benchmark(&benchmark);
    double findingMinValue = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    maxValueNode(tree);
    end_benchmark(&benchmark);
    double findingMaxValue = get_benchmark_result(&benchmark);

    int dataset[DATASET][2];
    for (int i = 0; i < DATASET; i++) {
        dataset[i][0] = rand() % KEYS;
    }

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int insertKey = dataset[i][0];
        createNode(insertKey);
    }
    end_benchmark(&benchmark);
    double insertionTime = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int searchKey = dataset[i][0];
        searchBinaryTree(tree, searchKey);
    }
    end_benchmark(&benchmark);
    double searchTime = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int deleteKey = dataset[i][0];
        deleteByKey(tree, deleteKey);
    }
    end_benchmark(&benchmark);
    double deleteTime = get_benchmark_result(&benchmark);

    start_benchmark(&benchmark);
    for (int i = 0; i < DATASET; i++) {
        int insertKey = dataset[i][0];
        createNode(insertKey);
        int maxTreshold = 0;
        int initialTruePositive = 0;
        int initialFalseNegative = 0;
        int initialFalsePositive = 0;
        detectBinaryAnomaly(tree, maxTreshold, &initialTruePositive, &initialFalsePositive, &initialFalseNegative);
    }
    end_benchmark(&benchmark);
    double findingAnomalies = get_benchmark_result(&benchmark);

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

    printf("Measuring finding minimum key: %f seconds \n", findingMinValue);
    printf("Measuring finding maximum key: %f seconds \n", findingMaxValue);
    printf("Measuring insertion time: %f seconds \n", insertionTime);
    printf("Measuring searching time: %f seconds \n", searchTime);
    printf("Measuring deletion time %f seconds \n", deleteTime);
    printf("Measuring find anomalies: %f seconds \n", findingAnomalies);

    freeTree(tree);

    return 0;
}
