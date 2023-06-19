#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "utils.h"

double getRandomNumber(int minValue, int maxValue)
{
    return minValue + (double)rand() / RAND_MAX * (maxValue - minValue);
}

void startBenchmark(BenchmarkResult *result)
{
    result->start = clock();
}

void endBenchmark(BenchmarkResult *result)
{
    result->end = clock();
    result->duration = (double)(result->end - result->start) / CLOCKS_PER_SEC;
}

double getBenchmarkResult(BenchmarkResult *result)
{
    return result->duration;
}
