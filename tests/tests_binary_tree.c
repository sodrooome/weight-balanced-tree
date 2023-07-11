#include <stdio.h>

#include "../include/tree.h"
#include "../include/utils.h"

void binaryTreeTests() {
    TreeNode *root = create_node(5);
    insert_binary_tree(root, 3);
    insert_binary_tree(root, 5);
    insert_binary_tree(root, 4);
    insert_binary_tree(root, 8);
    insert_binary_tree(root, 2);
    insert_binary_tree(root, 6);

    TreeNode *minValue = min_value_node(root);
    TreeNode *maxValue = max_value_node(root);

    if (root != NULL && root->numOfAnomaly == 0) {
        // for temporary, i will increment the num of anomaly
        // if the tree node can't capture the anomalies (or there's no anomaly at all)
        // so it can be calculated by the evaluation metrics model
        root->numOfAnomaly = 1;
    } else {
        root->numOfAnomaly = 0;
    }

    printf("=== Binary Tree Tests === \n");
    // Tree node is empty since i dont create the tree
    // and inserting all values into each node of tree
    if (maxValue == NULL) {
        handle_errors(0);
    } else {
        int expectation = 8;
        printf("Assertion for Max value is: %d, and the expectation is: %i \n", maxValue->key, expectation);
    }

    if (minValue == NULL) {
        handle_errors(0);
    } else {
        int expectation = 2;
        printf("Assertion for Min value is: %d, and the expectation is: %i \n", minValue->key, expectation);
    }

    int searchKey = 4;
    int result = search_binary_tree(root, searchKey);
    printf("Assertion for searching %i %s \n", searchKey, result ? "Is Found" : "Not Found");

    searchKey = 11;
    result = search_binary_tree(root, searchKey);
    printf("Assertion for searching %i %s \n", searchKey, result ? "Is Found" : "Not Found");

    int deleteKey = 4;
    root = delete_by_key(root, deleteKey);
    result = search_binary_tree(root, deleteKey);
    printf("Assertion for deletion %i, status is: %s \n", deleteKey, result ? "Not Removed" : "Removed");

    int initialTruePositive = 0;
    int initialTrueNegative = 0;
    int initialFalsePositive = 0;
    int initialFalseNegative = 0;

    int maxThreshold = 0;
    int findAnomaly =
        detect_binary_anomaly(root, maxThreshold, &initialTruePositive, &initialFalsePositive, &initialFalseNegative);
    printf("Anomaly score is: %i and the weight is: %i \n", root->numOfAnomaly, root->weight);

    float precisionResult = calculate_precision(initialTruePositive, initialFalsePositive);
    float accuracyResult =
        calculate_accuracy(initialTruePositive, initialTrueNegative, initialFalsePositive, initialFalseNegative);
    float recallResult = calculate_recall(initialTruePositive, initialFalseNegative);
    float f1ScoreResult = calculate_f1_score(precisionResult, recallResult);

    printf("Precision result: %2.f%% \n", precisionResult * 100);
    printf("Recall result: %2.f%% \n", recallResult * 100);
    printf("F1 Score result: %2.f%% \n", f1ScoreResult * 100);
    printf("Accuracy result: %2.f%% \n", accuracyResult * 100);

    free_tree(root);
}
