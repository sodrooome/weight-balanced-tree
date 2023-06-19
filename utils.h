#ifndef UTILS_H
#define UTILS_H

typedef struct
{
    int minValue;
    int maxValue;
} DataPoint;

typedef struct
{
    clock_t start;
    clock_t end;
    double duration;
} BenchmarkResult;

double getRandomNumber(int minValue, int maxValue);
double getBenchmarkResult(BenchmarkResult *result);
void startBenchmark(BenchmarkResult *result);
void endBenchmark(BenchmarkResult *result);

#endif
