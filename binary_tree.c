#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include <string.h>
#include <errno.h>
#include "tree.h"
#include "utils.h"

#define DATASET 10000
#define KEYS 10000

int searchBinaryTree(struct TreeNode *root, int key)
{
    if (root == NULL || root->key == key)
    {
        return root != NULL;
    }

    if (key < root->key)
    {
        return searchBinaryTree(root->left, key);
    }
    else
    {
        return searchBinaryTree(root->right, key);
    }
}

struct TreeNode *createNode(int key)
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    if (node == NULL)
    {
        fprintf(stderr, "Failed to allocate the memory into tree node %s \n", strerror(errno));
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->weight = 1;
    return node;
}

struct TreeNode *minValueNode(struct TreeNode *root)
{
    struct TreeNode *current = root;

    while (current && current->left != NULL)
    {
        current = current->left;
    }

    return current;
}

// main source: https://www.techiedelight.com/deletion-from-bst/
struct TreeNode *deleteByKey(struct TreeNode *root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (key < root->key)
    {
        root->left = deleteByKey(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = deleteByKey(root->right, key);
    }
    else
    {
        if (root->left == NULL)
        {
            struct TreeNode *tempRoot = root->right;
            free(root);
            return tempRoot;
        }
        else
        {
            struct TreeNode *tempRoot = root->left;
            free(root);
            return tempRoot;
        }

        struct TreeNode *tempRoot = minValueNode(root->right);
        root->key = tempRoot->key;
        root->right = deleteByKey(root->right, tempRoot->key);
    }

    return root;
}

struct TreeNode *insertBinaryTree(struct TreeNode *root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (key < root->key)
    {
        return insertBinaryTree(root->left, key);
    }
    else if (key > root->key)
    {
        return insertBinaryTree(root->right, key);
    }
    else
    {
        root->weight += 1;
    }

    root->weight += 1;
    return root;
}

void detectBinaryAnomaly(struct TreeNode *root, int threshold)
{
    if (root == NULL)
    {
        return;
    }

    if (root->weight > threshold)
    {
        printf("Anomaly is being detected with the weight of tree is %d\n", root->weight);
    }

    detectBinaryAnomaly(root->left, threshold);
    detectBinaryAnomaly(root->right, threshold);
}

void freeTree(struct TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void traversal(struct TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    traversal(root->right);
    printf("Traversal Binary Tree: %i", root->key);
    traversal(root->left);
}

void binaryTreeTests()
{
    struct TreeNode *root = createNode(5);
    insertBinaryTree(root, 3);
    insertBinaryTree(root, 5);
    insertBinaryTree(root, 4);
    insertBinaryTree(root, 8);
    insertBinaryTree(root, 2);
    insertBinaryTree(root, 6);

    printf("=== Binary Tree Tests === \n");
    int searchKey = 4;
    int result = searchBinaryTree(root, searchKey);
    printf("Assertion for searching %i %s \n", searchKey, result ? "Found" : "Not Found");

    searchKey = 11;
    result = searchBinaryTree(root, searchKey);
    printf("Assertion for searching %i %s \n", searchKey, result ? "Found" : "Not Found");

    int deleteKey = 4;
    root = deleteByKey(root, deleteKey);
    result = searchBinaryTree(root, deleteKey);
    printf("Assertion for deletion %i %s \n", deleteKey, result ? "Found" : "Not Found");

    int maxThreshold = 3;
    detectBinaryAnomaly(root, maxThreshold);
    printf("Assertion for detecting anomaly %i \n", maxThreshold);

    freeTree(root);
}

int main()
{

    struct BenchmarkResult benchmark;
    // CURL *request;
    // CURLcode response;

    int dataset[DATASET][2];
    for (int i = 0; i < DATASET; i++)
    {
        dataset[i][0] = rand() % KEYS;
    }

    struct TreeNode *tree;

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int insertKey = dataset[i][0];
        createNode(insertKey);
    }
    endBenchmark(&benchmark);
    double insertionTime = getBenchmarkResult(&benchmark);

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int searchKey = dataset[i][0];
        searchBinaryTree(tree, searchKey);
    }
    endBenchmark(&benchmark);
    double searchTime = getBenchmarkResult(&benchmark);

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int deleteKey = dataset[i][0];
        deleteByKey(tree, deleteKey);
    }
    endBenchmark(&benchmark);
    double deleteTime = getBenchmarkResult(&benchmark);

    startBenchmark(&benchmark);
    for (int i = 0; i < DATASET; i++)
    {
        int insertKey = dataset[i][0];
        createNode(insertKey);
        int maxTreshold = 10;
        detectBinaryAnomaly(tree, maxTreshold);
    }
    endBenchmark(&benchmark);
    double findingAnomalies = getBenchmarkResult(&benchmark);

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

    printf("Measuring insertion time: %f seconds \n", insertionTime);
    printf("Measuring searching time: %f seconds \n", searchTime);
    printf("Measuring deletion time %f seconds \n", deleteTime);
    printf("Measuring find anomalies: %f seconds \n", findingAnomalies);

    freeTree(tree);

    return 0;
}
