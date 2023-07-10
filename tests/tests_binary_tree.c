#include <stdio.h>

#include "../include/tree.h"
#include "../include/utils.h"

void binaryTreeTests() {
    TreeNode *root = createNode(5);
    insertBinaryTree(root, 3);
    insertBinaryTree(root, 5);
    insertBinaryTree(root, 4);
    insertBinaryTree(root, 8);
    insertBinaryTree(root, 2);
    insertBinaryTree(root, 6);

    TreeNode *minValue = minValueNode(root);
    TreeNode *maxValue = maxValueNode(root);

    printf("=== Binary Tree Tests === \n");
    // Tree node is empty since i dont create the tree
    // and inserting all values into each node of tree
    if (maxValue == NULL) {
        handleErrors(0);
    } else {
        int expectation = 8;
        printf("Assertion for Max value is: %d, and the expectation is: %i \n", maxValue->key, expectation);
    }

    if (minValue == NULL) {
        handleErrors(0);
    } else {
        int expectation = 2;
        printf("Assertion for Min value is: %d, and the expectation is: %i \n", minValue->key, expectation);
    }

    int searchKey = 4;
    int result = searchBinaryTree(root, searchKey);
    printf("Assertion for searching %i %s \n", searchKey, result ? "Is Found" : "Not Found");

    searchKey = 11;
    result = searchBinaryTree(root, searchKey);
    printf("Assertion for searching %i %s \n", searchKey, result ? "Is Found" : "Not Found");

    int deleteKey = 4;
    root = deleteByKey(root, deleteKey);
    result = searchBinaryTree(root, deleteKey);
    printf("Assertion for deletion %i, status is: %s \n", deleteKey, result ? "Not Removed" : "Removed");

    int initialTruePositive = 0;
    int initialTrueNegative = 0;
    int initialFalsePositive = 0;
    int initialFalseNegative = 0;

    int maxThreshold = 0;
    int findAnomaly =
        detectBinaryAnomaly(root, maxThreshold, &initialTruePositive, &initialFalsePositive, &initialFalseNegative);
    if (findAnomaly < 0) {
        handleErrors(findAnomaly);
        printf("Anomaly weight that found is: %i \n", root->weight);
    } else {
        printf("Anomaly score and weight : %i %i \n", root->weight, root->numOfAnomaly);
        printf("No anomalies that being found, result of finding anomalies is: %i \n", findAnomaly);
    }

    float precisionResult = calculatePrecision(initialTruePositive, initialFalsePositive);
    float accuracyResult =
        calculateAccuracy(initialTruePositive, initialTrueNegative, initialFalsePositive, initialFalseNegative);
    float recallResult = calculateRecall(initialTruePositive, initialFalseNegative);
    float f1ScoreResult = calculateF1Score(precisionResult, recallResult);

    printf("Precision result: %2.f%% \n", precisionResult * 100);
    printf("Recall result: %2.f%% \n", recallResult * 100);
    printf("F1 Score result: %2.f%% \n", f1ScoreResult * 100);
    printf("Accuracy result: %2.f%% \n", accuracyResult * 100);

    freeTree(root);
}
