#include <random>
#include <iostream>
#include "kMeansClustering.h"

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
    bool updateStopped;

    const int pointsSize = points.size();
    const int clusterSize = clusters.size();

    do {

        iter++;
        // Reset flag
        updateStopped = true;

#pragma omp parallel for default(none) firstprivate(pointsSize, clusterSize) shared(updateStopped, points, clusters)
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
                    updateStopped = false;
            }

            clusters[currentMinClusterId].addPoint(*point);

        }

        // Update
        for (int i = 0; i < clusterSize; i++) {
            clusters[i].updateCentroid();
        }


    } while (!updateStopped && iter < maxIters);

    return clusters;
}
