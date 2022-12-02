#include <random>
#include <iostream>
#include "kMeansClustering.h"

std::vector<Cluster> randomInit(int k, std::vector<Point> &points) {
    std::vector<Cluster> clusters;
    clusters.reserve(k);

    std::random_device rd;
    srand(rd());
    for (int i = 0; i < k; i++) {
        clusters.emplace_back(i, points[rand() % points.size()]);
    }

    return clusters;
}

std::vector<Cluster> kMeansPlusPlusInit(int k, std::vector<Point> &points) {
    std::vector<Cluster> clusters;
    clusters.reserve(k);

    std::random_device rd;
    std::default_random_engine generator(rd());
    srand(rd());

    std::vector<Point> centroids;
    centroids.reserve(k);

    centroids.push_back(points[rand() % points.size()]);
    clusters.emplace_back(0, centroids[0]);

    for (int i = 1; i < k; i++) {
        std::vector<float> minDists(points.size(), 0.0);
        float minDistsSum = 0;
//#pragma omp parallel for reduction(+:minDistsSum) default(none) shared(points, minDists, centroids)
        for (int j = 0; j < points.size(); j++) {
            float currentMinDist = std::numeric_limits<float>::max();
            for (int z = 0; z < centroids.size(); z++) {
                float dist = points[j].dist(centroids[z]);
                if (dist < currentMinDist) {
                    currentMinDist = dist;
                }
            }
            minDists[j] = currentMinDist;
            minDistsSum += currentMinDist;
        }

//#pragma omp for simd
        for (int j = 0; j < minDists.size(); j++) {
            minDists[j] /= minDistsSum;
        }

        std::discrete_distribution<int> distribution(minDists.begin(), minDists.end());
        Point centroid = points[distribution(generator)];

        centroids.push_back(centroid);
        clusters.emplace_back(i, centroid);
    }

    return clusters;
}

std::vector<Cluster> kMeansClustering(int k, std::vector<Point> &points, int maxIters) {

    std::vector<Cluster> clusters = randomInit(k, points);
//    std::vector<Cluster> clusters = kMeansPlusPlusInit(k, points);

    int iter = 0;
    bool updateStopped;

    const int pointsSize = points.size();
    const int clusterSize = clusters.size();

    do {

        iter++;

        // Reset flag
        updateStopped = true;

        // Reset lists
        for (int i = 0; i < clusters.size(); i++) {
            clusters[i].points.clear();
        }

#pragma omp parallel default(none) firstprivate(pointsSize, clusterSize) shared(updateStopped, points, clusters)
        {
            // Assignment
#pragma omp for
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
            }

            // Update
#pragma omp for
            for (int i = 0; i < clusterSize; i++) {
                Cluster *cluster = &clusters[i];

                for (int j = 0; j < pointsSize; j++) {
                    Point *point = &points[j];

                    if (point->clusterId == cluster->id) {
                        cluster->addPoint(point);
                    }
                }
                cluster->updateCentroid();
            }

        }

    } while (!updateStopped && iter < maxIters);

    return clusters;
}