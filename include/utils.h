#ifndef UTILS_H
#define UTILS_H

#include <time.h>

typedef struct {
    int minValue;
    int maxValue;
} DataPoint;

typedef struct {
    clock_t start;
    clock_t end;
    double duration;
} BenchmarkResult;

double getRandomNumber(int minValue, int maxValue);
double getBenchmarkResult(BenchmarkResult *result);
float calculatePrecision(int truePositive, int falsePositive);
float calculateRecall(int truePositive, int falseNegative);
float calculateF1Score(float precision, float recall);
void anomalySorting(double *anomalyScores, int *actualLabels, int numOfSamples);
void startBenchmark(BenchmarkResult *result);
void endBenchmark(BenchmarkResult *result);
void handleErrors(int returnedCode);

#endif
