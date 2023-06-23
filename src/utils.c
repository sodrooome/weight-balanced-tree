#include "../include/utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double getRandomNumber(int minValue, int maxValue) {
    return minValue + (double)rand() / RAND_MAX * (maxValue - minValue);
}

void startBenchmark(BenchmarkResult *result) {
    result->start = clock();
}

void endBenchmark(BenchmarkResult *result) {
    result->end = clock();
    result->duration = (double)(result->end - result->start) / CLOCKS_PER_SEC;
}

void handleErrors(int returnedCode) {
    switch (returnedCode) {
        case -1:
            printf("Anomaly is being detected \n");
            break;
        case 0:
            printf("TreeNode is not initialized yet \n");
            break;
        default:
            printf("Unexpected errors occured \n");
    }
}

double getBenchmarkResult(BenchmarkResult *result) {
    return result->duration;
}
