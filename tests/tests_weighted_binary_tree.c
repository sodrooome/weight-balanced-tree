#include <stdio.h>
#include <stdlib.h>

#include "../include/tree.h"

void weightBalancedBinaryTreeTests() {
    WeightBalancedTree *tree = newWeightBalancedTree();
    TreeNode *root = NULL;
    insert(tree, 1, 0);
    insert(tree, 3, 0);
    insert(tree, 4, 0);
    insert(tree, 6, 0);
    insert(tree, 2, 0);
    insert(tree, 11, 1);

    printf("=== Weighted Binary Tree Tests === \n");
    int searchKey = 1;
    int result = searchOperation(tree->root, searchKey);
    printf("Assertion for searching key %i %s \n", searchKey, result ? "Found" : "Not Found");

    searchKey = 19;
    result = searchOperation(tree->root, searchKey);
    printf("Assertion for searching key (invalid key) %i %s \n", searchKey, result ? "Is Found" : "Not Found");

    int delKey = 11;
    tree->root = deleteByKey(tree->root, delKey);

    // this will lead to other segmentation fault errors
    // TODO: fix what causing this error before the deletion process (rebalance process)
    // printf("Assertion for deleting key %d \n", tree->root->key);
    searchKey = 11;
    result = searchOperation(tree->root, searchKey);
    printf("Assertion after deletion the key: %i should be removed, status: %s \n", searchKey,
           result ? "Removed" : "Not Removed");

    int initialTruePositive = 0;
    int initialTrueNegative = 0;
    int initialFalsePositive = 0;
    int initialFalseNegative = 0;

    int maxThreshold = 0;
    int findAnomaly = constantDetectAnomaly(tree->root, maxThreshold, &initialTruePositive, &initialFalsePositive,
                                            &initialFalseNegative);
    if (findAnomaly < 0) {
        handle_errors(findAnomaly);
    } else {
        printf("Weight: %i \n", tree->root->weight);
        printf("No anomalies that being found, result of finding anomalies is: %i \n", findAnomaly);
    }

    float detectPrecision = calculate_precision(initialTruePositive, initialFalsePositive);
    float detectRecall = calculate_recall(initialTruePositive, initialFalseNegative);
    float detectF1Score = calculate_f1_score(detectPrecision, detectRecall);
    float detectAccuracy =
        calculate_accuracy(initialTruePositive, initialTrueNegative, initialFalsePositive, initialFalseNegative);
    printf("Precision result (constant-time): %.2f%%\n", detectPrecision * 100);
    printf("Recall result (constant-time): %2.f%%\n", detectRecall * 100);
    printf("F1 score result (constant-time): %2.f%%\n", detectF1Score * 100);
    printf("Accuracy result (constant-time): %2.f%%\n", detectAccuracy * 100);

    int setOfFeatures[1] = {2};
    maxThreshold = 0;
    int *features = setOfFeatures;
    int numOfFeatures = 1;
    // debug purposes, might've removed later on
    // printf("Weight of root: %i \n", tree->root->weight);
    // manual calculation -> tp = 1 (since the value of features is in the TreeNode)
    // and fp = 0: 1 / (1 + 0) -> 1.0 (100 %)
    detectAnomalies(tree, maxThreshold, features, numOfFeatures, &initialTruePositive, &initialFalsePositive,
                    &initialFalseNegative);
    float precision = calculate_precision(initialTruePositive, initialFalsePositive);
    float recall = calculate_recall(initialTruePositive, initialFalseNegative);

    // calculate the F1 score based on the precision and recall scores
    float f1Score = calculate_f1_score(precision, recall);

    float accuracy =
        calculate_accuracy(initialTruePositive, initialTrueNegative, initialFalsePositive, initialFalseNegative);

    // if the anomaly score is exceeds the max threshold, it would be identified the tree as anomaly
    int anomalyScores = tree->root->weight;
    if (anomalyScores > maxThreshold) {
        printf("Anomaly is being detected with the score: %i \n", anomalyScores);
    } else {
        printf("No anomalies \n");
    }
    printf("F1 result: %.2f%%\n", f1Score * 100);
    printf("Precision result: %.2f%%\n", precision * 100);
    printf("Recall result: %.2f%%\n", recall * 100);
    printf("Accuracy result: %.2f%%\n", accuracy * 100);

    freeTree(tree->root);
}
