#include <stdio.h>
#include <stdlib.h>

#include "../include/tree.h"

void weightBalancedBinaryTreeTests() {
    WeightBalancedTree *tree = new_weight_balanced_tree();
    TreeNode *root = NULL;
    insert(tree, 1, 0);
    insert(tree, 3, 0);
    insert(tree, 4, 0);
    insert(tree, 6, 0);
    insert(tree, 2, 0);
    insert(tree, 11, 1);

    printf("=== Weighted Binary Tree Tests === \n");
    int search_key = 1;
    int result = search_operation(tree->root, search_key);
    printf("Assertion for searching key %i %s \n", search_key, result ? "Found" : "Not Found");

    search_key = 19;
    result = search_operation(tree->root, search_key);
    printf("Assertion for searching key (invalid key) %i %s \n", search_key, result ? "Is Found" : "Not Found");

    int del_key = 11;
    tree->root = delete_by_key(tree->root, del_key);

    // this will lead to other segmentation fault errors
    // TODO: fix what causing this error before the deletion process (rebalance process)
    // printf("Assertion for deleting key %d \n", tree->root->key);
    search_key = 11;
    result = search_operation(tree->root, search_key);
    printf("Assertion after deletion the key: %i should be removed, status: %s \n", search_key,
           result ? "Removed" : "Not Removed");

    int initial_true_positive = 0;
    int initial_true_negative = 0;
    int initial_false_positive = 0;
    int initial_false_negative = 0;

    int max_threshold = 0;
    int find_anomaly = constant_detect_anomaly(tree->root, max_threshold, &initial_true_positive,
                                               &initial_false_positive, &initial_false_negative);
    if (find_anomaly < 0) {
        handle_errors(find_anomaly);
    } else {
        printf("Weight: %i \n", tree->root->weight);
        printf("No anomalies that being found, result of finding anomalies is: %i \n", find_anomaly);
    }

    float detect_precision = calculate_precision(initial_true_positive, initial_false_positive);
    float detect_recall = calculate_recall(initial_true_positive, initial_false_negative);
    float detect_f1_score = calculate_f1_score(detect_precision, detect_recall);
    float detect_accuracy = calculate_accuracy(initial_true_positive, initial_true_negative, initial_false_positive,
                                               initial_false_negative);
    printf("Precision result (constant-time): %.2f%%\n", detect_precision * 100);
    printf("Recall result (constant-time): %2.f%%\n", detect_recall * 100);
    printf("F1 score result (constant-time): %2.f%%\n", detect_f1_score * 100);
    printf("Accuracy result (constant-time): %2.f%%\n", detect_accuracy * 100);

    int set_of_features[1] = {2};
    max_threshold = 0;
    int *features = set_of_features;
    int num_of_features = 1;
    // debug purposes, might've removed later on
    // printf("Weight of root: %i \n", tree->root->weight);
    // manual calculation -> tp = 1 (since the value of features is in the TreeNode)
    // and fp = 0: 1 / (1 + 0) -> 1.0 (100 %)
    detect_anomalies(tree, max_threshold, features, num_of_features, &initial_true_positive, &initial_false_positive,
                     &initial_false_negative);
    float precision = calculate_precision(initial_true_positive, initial_false_positive);
    float recall = calculate_recall(initial_true_positive, initial_false_negative);

    // calculate the F1 score based on the precision and recall scores
    float f1_score = calculate_f1_score(precision, recall);

    float accuracy = calculate_accuracy(initial_true_positive, initial_true_negative, initial_false_positive,
                                        initial_false_negative);

    // if the anomaly score is exceeds the max threshold, it would be identified the tree as anomaly
    int anomaly_scores = tree->root->weight;
    if (anomaly_scores > max_threshold) {
        printf("Anomaly is being detected with the score: %i \n", anomaly_scores);
    } else {
        printf("No anomalies \n");
    }
    printf("F1 result: %.2f%%\n", f1_score * 100);
    printf("Precision result: %.2f%%\n", precision * 100);
    printf("Recall result: %.2f%%\n", recall * 100);
    printf("Accuracy result: %.2f%%\n", accuracy * 100);

    free_tree(tree->root);
}
