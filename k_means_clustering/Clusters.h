#ifndef K_MEANS_CLUSTERING_CLUSTERS_H
#define K_MEANS_CLUSTERING_CLUSTERS_H

#include <vector>

struct Clusters {
    int size = 0;
    std::vector<float> centroidX;
    std::vector<float> centroidY;
    std::vector<int> id;
    std::vector<int> clusterSize;
    std::vector<float> centroidXTemp;
    std::vector<float> centroidYTemp;
};


#endif //K_MEANS_CLUSTERING_CLUSTERS_H
