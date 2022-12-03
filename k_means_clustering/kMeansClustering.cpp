#include <random>
#include <iostream>
#include "kMeansClustering.h"

// ---------------------------------- AoS --------------------------- //

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

// ---------------------------------- SoA --------------------------- //

Clusters randomInit(int k, Points &points) {
    Clusters clusters;

    clusters.size = k;
    clusters.centroidX.reserve(k);
    clusters.centroidY.reserve(k);
    clusters.id.reserve(k);
    clusters.clusterSize.reserve(k);

    std::random_device rd;
    srand(rd());
    for (int i = 0; i < k; i++) {
        int j = rand() % points.size;
        clusters.centroidX.push_back(points.x[j]);
        clusters.centroidY.push_back(points.y[j]);
        clusters.id.push_back(i);
        clusters.clusterSize.push_back(0);
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
    bool updateStopped;

    do {

        iter++;

        // Reset flag
        updateStopped = true;

#pragma omp parallel default(none) shared(updateStopped, points, clusters)
        {
            // Assignment
#pragma omp for
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
                    updateStopped = false;
                }
            }

            // Update
#pragma omp for
            for (int i = 0; i < clusters.size; i++) {
                clusters.centroidX[i] = 0;
                clusters.centroidY[i] = 0;
                clusters.clusterSize[i] = 0;

                for (int j = 0; j < points.size; j++) {
                    if (points.clusterId[j] == clusters.id[i]) {
                        clusters.centroidX[i] += points.x[j];
                        clusters.centroidY[i] += points.y[j];
                        clusters.clusterSize[i]++;
                    }
                }

                clusters.centroidX[i] /= clusters.clusterSize[i];
                clusters.centroidY[i] /= clusters.clusterSize[i];
            }

        }

    } while (!updateStopped && iter < maxIters);

    return clusters;
}