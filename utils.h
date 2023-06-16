#ifndef UTILS_H
#define UTILS_H

struct DataPoint
{
    int minValue;
    int maxValue;
};

struct BenchmarkResult
{
    clock_t start;
    clock_t end;
    double duration;
};

double getRandomNumber(int minValue, int maxValue);
double getBenchmarkResult(struct BenchmarkResult *result);
void startBenchmark(struct BenchmarkResult *result);
void endBenchmark(struct BenchmarkResult *result);

#endif
