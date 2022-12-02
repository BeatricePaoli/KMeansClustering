#ifndef K_MEANS_CLUSTERING_UTILS_H
#define K_MEANS_CLUSTERING_UTILS_H

#include <string>
#include <vector>
#include "Cluster.h"

std::vector<std::string> getDatasetFilenames(const std::string& datasetsDirectory);
std::vector<Point> readPointsFromCsv(const std::string& filename, char delimiter);
void writeClustersToCsv(const std::vector<Cluster>& clusters, const std::string& filename, char delimiter);

#endif //K_MEANS_CLUSTERING_UTILS_H
