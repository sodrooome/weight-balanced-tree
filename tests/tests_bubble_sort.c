#include "../include/utils.h"

void bubbleSortAlgorithmTests() {
    double anomalyScore[] = {1.5, 2.0, 3.5, 4.5};
    int actualLables[] = {1, 2, 3, 4};
    int numOfSamples = 4;

    anomalySorting(anomalyScore, actualLables, numOfSamples);

    for (int i = 0; i < numOfSamples; i++) {
        if (anomalyScore[i] != actualLables[i]) {
            // just for checking whether there's a value that
            // both are equals in the anomaly score and actual values
            printf("Expecting the actual lables shouldn't be match in the anomaly score list \n");
            break;
        }
    }

    int size = sizeof(anomalyScore) / sizeof(anomalyScore[0]);
    if (size == 4) {
        printf("Data is match \n");
    } else {
        printf("Data is not match : %i \n", size);
    }
}
