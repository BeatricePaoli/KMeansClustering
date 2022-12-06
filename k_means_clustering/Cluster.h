#ifndef K_MEANS_CLUSTERING_CLUSTER_H
#define K_MEANS_CLUSTERING_CLUSTER_H

#include "Point.h"

class Cluster {
public:
    explicit Cluster(int id, Point centroid);

    void addPoint(Point point);
    void updateCentroid();

    int id;
    Point centroid;

private:
    float tempSumX;
    float tempSumY;
    int size;
};

#endif //K_MEANS_CLUSTERING_CLUSTER_H
