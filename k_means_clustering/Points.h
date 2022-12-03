#ifndef K_MEANS_CLUSTERING_POINTS_H
#define K_MEANS_CLUSTERING_POINTS_H

#include <vector>

struct Points {
    int size = 0;
    std::vector<float> x;
    std::vector<float> y;
    std::vector<int> clusterId;
};

#endif //K_MEANS_CLUSTERING_POINTS_H
