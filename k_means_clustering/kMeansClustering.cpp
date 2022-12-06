#include <random>
#include <iostream>
#include "kMeansClustering.h"

// ---------------------------------- AoS --------------------------- //

std::vector<Cluster> randomInit(int k, std::vector<Point> &points) {
    std::vector<Cluster> clusters;
    clusters.reserve(k);

//    std::random_device rd;
//    srand(rd());
    srand(0);
    for (int i = 0; i < k; i++) {
        clusters.emplace_back(i, points[rand() % points.size()]);
    }

    return clusters;
}

std::vector<Cluster> kMeansClustering(int k, std::vector<Point> &points, int maxIters) {

    std::vector<Cluster> clusters = randomInit(k, points);

    int iter = 0;
//    bool updateStopped;

    const int pointsSize = points.size();
    const int clusterSize = clusters.size();

    do {

        iter++;
        // Reset flag
//        updateStopped = true;

#pragma omp parallel for default(none) firstprivate(pointsSize, clusterSize) shared(points, clusters)
        for (int i = 0; i < pointsSize; i++) {
            Point *point = &points[i];

            float currentMinDist = std::numeric_limits<float>::max();
            int currentMinClusterId = -1;

            for (int j = 0; j < clusterSize; j++) {
                Cluster *cluster = &clusters[j];

                float dist = point->dist(cluster->centroid);
                if (dist < currentMinDist) {
                    currentMinDist = dist;
                    currentMinClusterId = cluster->id;
                }
            }

            if (point->clusterId != currentMinClusterId) {
                point->clusterId = currentMinClusterId;
//                    updateStopped = false;
            }

            clusters[currentMinClusterId].addPoint(*point);

        }

        // Update
        for (int i = 0; i < clusterSize; i++) {
            clusters[i].updateCentroid();
        }


    } while (iter < maxIters); // !updateStopped &&

    return clusters;
}

// ---------------------------------- SoA --------------------------- //

Clusters randomInit(int k, Points &points) {
    Clusters clusters;

    clusters.size = k;
    clusters.centroidX.reserve(k);
    clusters.centroidY.reserve(k);
    clusters.id.reserve(k);
    clusters.clusterSize.reserve(k);
    clusters.centroidXTemp.reserve(k);
    clusters.centroidYTemp.reserve(k);

//    std::random_device rd;
//    srand(rd());
    srand(0);
    for (int i = 0; i < k; i++) {
        int j = rand() % points.size;
        clusters.centroidX.push_back(points.x[j]);
        clusters.centroidY.push_back(points.y[j]);
        clusters.id.push_back(i);
        clusters.clusterSize.push_back(0);
        clusters.centroidXTemp.push_back(0);
        clusters.centroidYTemp.push_back(0);
    }

    return clusters;
}

float distance(float x1, float y1, float x2, float y2) {
    float sum = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    return sqrtf(sum);
}

Clusters kMeansClustering(int k, Points &points, int maxIters) {
    Clusters clusters = randomInit(k, points);

    int iter = 0;
//    bool updateStopped;

    do {

        iter++;

        // Reset flag
//        updateStopped = true;

        // Assignment
#pragma omp parallel for default(none) shared(points, clusters)
        for (int i = 0; i < points.size; i++) {
            float currentMinDist = std::numeric_limits<float>::max();
            int currentMinClusterId = -1;

            for (int j = 0; j < clusters.size; j++) {
                float dist = distance(points.x[i], points.y[i], clusters.centroidX[j], clusters.centroidY[j]);
                if (dist < currentMinDist) {
                    currentMinDist = dist;
                    currentMinClusterId = clusters.id[j];
                }
            }

            if (points.clusterId[i] != currentMinClusterId) {
                points.clusterId[i] = currentMinClusterId;
//                    updateStopped = false;
            }

#pragma omp atomic
            clusters.centroidXTemp[currentMinClusterId] += points.x[i];
#pragma omp atomic
            clusters.centroidYTemp[currentMinClusterId] += points.y[i];
#pragma omp atomic
            clusters.clusterSize[currentMinClusterId]++;
        }

        // Update
        for (int i = 0; i < clusters.size; i++) {
            clusters.centroidX[i] = clusters.centroidXTemp[i] / clusters.clusterSize[i];
            clusters.centroidY[i] = clusters.centroidYTemp[i] / clusters.clusterSize[i];

            clusters.centroidXTemp[i] = 0;
            clusters.centroidYTemp[i] = 0;
            clusters.clusterSize[i] = 0;
        }


    } while (iter < maxIters); // !updateStopped &&

    return clusters;
}