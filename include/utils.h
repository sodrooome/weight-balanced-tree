#ifndef UTILS_H
#define UTILS_H

#include <time.h>

typedef struct {
    int min_value;
    int max_value;
} DataPoint;

typedef struct {
    clock_t start;
    clock_t end;
    double duration;
} BenchmarkResult;

double get_random_number(int min_value, int max_value);
double get_benchmark_result(BenchmarkResult *result);
float calculate_precision(int true_positive, int false_positive);
float calculate_recall(int true_positive, int false_negative);
float calculate_f1_score(float precision, float recall);
float calculate_accuracy(int true_positive, int true_negative, int false_positive, int false_negative);
void anomaly_sorting(double *anomaly_scores, int *actual_labels, int num_of_samples);
void start_benchmark(BenchmarkResult *result);
void end_benchmark(BenchmarkResult *result);
void handle_errors(int returned_code);

#endif
