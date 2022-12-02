#ifndef K_MEANS_CLUSTERING_KMEANSCLUSTERING_H
#define K_MEANS_CLUSTERING_KMEANSCLUSTERING_H

#include <vector>
#include "Cluster.h"

std::vector<Cluster> kMeansClustering(int k, std::vector<Point>& points, int maxIters = 20);

#endif //K_MEANS_CLUSTERING_KMEANSCLUSTERING_H
