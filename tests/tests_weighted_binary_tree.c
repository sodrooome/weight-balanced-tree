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
    tree->root = deleteByKey(root, delKey);

    // this will lead to other segmentation fault errors
    // TODO: fix what causing this error before the deletion process (rebalance process)
    printf("Assertion for deleting key %i \n", tree->root->key);
    searchKey = 11;
    result = searchOperation(tree->root, searchKey);
    printf("Assertion after deletion the key: %i should be removed, status: %s \n", searchKey,
           result ? "Removed" : "Not Removed");

    int maxThreshold = 7;
    constantDetectAnomaly(root, maxThreshold);
    printf("Detection anomaly with constant time \n");

    freeTree(tree->root);
}
