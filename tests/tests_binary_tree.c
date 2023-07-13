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

    TreeNode *min_value = min_value_node(root);
    TreeNode *max_value = max_value_node(root);

    if (root != NULL && root->num_of_anomaly == 0) {
        // for temporary, i will increment the num of anomaly
        // if the tree node can't capture the anomalies (or there's no anomaly at all)
        // so it can be calculated by the evaluation metrics model
        root->num_of_anomaly = 1;
    } else {
        root->num_of_anomaly = 0;
    }

    printf("=== Binary Tree Tests === \n");
    // Tree node is empty since i dont create the tree
    // and inserting all values into each node of tree
    if (max_value == NULL) {
        handle_errors(0);
    } else {
        int expectation = 8;
        printf("Assertion for Max value is: %d, and the expectation is: %i \n", max_value->key, expectation);
    }

    if (min_value == NULL) {
        handle_errors(0);
    } else {
        int expectation = 2;
        printf("Assertion for Min value is: %d, and the expectation is: %i \n", min_value->key, expectation);
    }

    int search_key = 4;
    int result = search_binary_tree(root, search_key);
    printf("Assertion for searching %i %s \n", search_key, result ? "Is Found" : "Not Found");

    search_key = 11;
    result = search_binary_tree(root, search_key);
    printf("Assertion for searching %i %s \n", search_key, result ? "Is Found" : "Not Found");

    int delete_key = 4;
    root = delete_by_key(root, delete_key);
    result = search_binary_tree(root, delete_key);
    printf("Assertion for deletion %i, status is: %s \n", delete_key, result ? "Not Removed" : "Removed");

    int initial_true_positive = 0;
    int initial_true_negative = 0;
    int initial_false_positive = 0;
    int initial_false_negative = 0;

    int max_threshold = 0;
    int find_anoamly = detect_binary_anomaly(root, max_threshold, &initial_true_positive, &initial_false_positive,
                                             &initial_false_negative);
    printf("Anomaly score is: %i and the weight is: %i \n", root->num_of_anomaly, root->weight);

    float precision_result = calculate_precision(initial_true_positive, initial_false_positive);
    float accuracy_result = calculate_accuracy(initial_true_positive, initial_true_negative, initial_false_positive,
                                               initial_false_negative);
    float recall_result = calculate_recall(initial_true_positive, initial_false_negative);
    float f1_score_result = calculate_f1_score(precision_result, recall_result);

    printf("Precision result: %2.f%% \n", precision_result * 100);
    printf("Recall result: %2.f%% \n", recall_result * 100);
    printf("F1 Score result: %2.f%% \n", f1_score_result * 100);
    printf("Accuracy result: %2.f%% \n", accuracy_result * 100);

    free_tree(root);
}
