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

// copy-paste from here https://www.programiz.com/dsa/bubble-sort with
// several adjustment... but yeah
void anomalySorting(double *anomalyScores, int *actualLabels, int numOfSamples) {
    for (int i = 0; i < numOfSamples - 1; i++) {
        for (int j = 0; j < numOfSamples - i - 1; j++) {
            if (anomalyScores[j] < anomalyScores[j + 1]) {
                double tempAnomalyScore = anomalyScores[j];
                anomalyScores[j] = anomalyScores[j + 1];
                anomalyScores[j + 1] = tempAnomalyScore;

                int tempLabels = actualLabels[j];
                actualLabels[j] = actualLabels[j + 1];
                actualLabels[j + 1] = tempLabels;
            }
        }
    }
}

// TODO: implement the AUC-ROC evaluation metrics using
// a simple bubble sort algorithm for sorting the anomaly score
void calculateAucRoc(double *anomalyScores, int *actualLabels, int numOfSamples) {
    anomalySorting(anomalyScores, actualLabels, numOfSamples);
}

double getBenchmarkResult(BenchmarkResult *result) {
    return result->duration;
}

float calculateAccuracy(int truePositive, int trueNegative, int falsePositive, int falseNegative) {
    return (float)(truePositive + trueNegative) / (truePositive + trueNegative + falsePositive + falseNegative);
}

float calculatePrecision(int truePositive, int falsePositive) {
    return (float)truePositive / (truePositive + falsePositive);
}

float calculateRecall(int truePositive, int falseNegative) {
    return (float)truePositive / (truePositive + falseNegative);
}

float calculateF1Score(float precision, float recall) {
    if (precision + recall == 0) {
        return 0;
    }

    return 2 * (precision * recall) / (precision + recall);
}
