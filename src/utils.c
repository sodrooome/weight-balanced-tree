#include "../include/utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double get_random_number(int min_value, int max_value) {
    return min_value + (double)rand() / RAND_MAX * (max_value - min_value);
}

void start_benchmark(BenchmarkResult *result) {
    result->start = clock();
}

void end_benchmark(BenchmarkResult *result) {
    result->end = clock();
    result->duration = (double)(result->end - result->start) / CLOCKS_PER_SEC;
}

void handle_errors(int returned_code) {
    switch (returned_code) {
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
void anomaly_sorting(double *anomaly_scores, int *actual_labels, int num_of_samples) {
    for (int i = 0; i < num_of_samples - 1; i++) {
        for (int j = 0; j < num_of_samples - i - 1; j++) {
            if (anomaly_scores[j] < anomaly_scores[j + 1]) {
                double temp_anomaly_score = anomaly_scores[j];
                anomaly_scores[j] = anomaly_scores[j + 1];
                anomaly_scores[j + 1] = temp_anomaly_score;

                int temp_labels = actual_labels[j];
                actual_labels[j] = actual_labels[j + 1];
                actual_labels[j + 1] = temp_labels;
            }
        }
    }
}

// TODO: implement the AUC-ROC evaluation metrics using
// a simple bubble sort algorithm for sorting the anomaly score
void calculate_aoc_roc(double *anomalyScores, int *actualLabels, int numOfSamples) {
    anomaly_sorting(anomalyScores, actualLabels, numOfSamples);
}

double get_benchmark_result(BenchmarkResult *result) {
    return result->duration;
}

float calculate_accuracy(int true_positive, int true_negative, int false_positive, int false_negative) {
    return (float)(true_positive + true_negative) / (true_positive + true_negative + false_positive + false_negative);
}

float calculate_precision(int true_positive, int false_positive) {
    return (float)true_positive / (true_positive + false_positive);
}

float calculate_recall(int true_positive, int false_negative) {
    return (float)true_positive / (true_positive + false_negative);
}

float calculate_f1_score(float precision, float recall) {
    if (precision + recall == 0) {
        return 0;
    }

    return 2 * (precision * recall) / (precision + recall);
}
