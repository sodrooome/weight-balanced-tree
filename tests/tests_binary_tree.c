#include "../include/tree.h"

void binaryTreeTests() {
    TreeNode *root = createNode(5);
    insertBinaryTree(root, 3);
    insertBinaryTree(root, 5);
    insertBinaryTree(root, 4);
    insertBinaryTree(root, 8);
    insertBinaryTree(root, 2);
    insertBinaryTree(root, 6);

    printf("=== Binary Tree Tests === \n");
    int searchKey = 4;
    int result = searchBinaryTree(root, searchKey);
    printf("Assertion for searching %i %s \n", searchKey, result ? "Is Found" : "Not Found");

    searchKey = 11;
    result = searchBinaryTree(root, searchKey);
    printf("Assertion for searching %i %s \n", searchKey, result ? "Is Found" : "Not Found");

    int deleteKey = 4;
    root = deleteByKey(root, deleteKey);
    result = searchBinaryTree(root, deleteKey);
    printf("Assertion for deletion %i %s \n", deleteKey, result ? "Is Found" : "Not Found");

    int maxThreshold = 3;
    detectBinaryAnomaly(root, maxThreshold);
    printf("Assertion for detecting anomaly %i \n", maxThreshold);

    freeTree(root);
}
