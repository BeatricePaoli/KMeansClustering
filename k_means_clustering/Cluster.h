#ifndef K_MEANS_CLUSTERING_CLUSTER_H
#define K_MEANS_CLUSTERING_CLUSTER_H

#include <vector>
#include <omp.h>
#include "Point.h"

class Cluster {
public:
    explicit Cluster(int id, Point centroid);

    void addPoint(Point *point);
    void updateCentroid();

    int id;
    Point centroid;
    std::vector<Point> points;

private:
    float tempSumX;
    float tempSumY;
};

#endif //K_MEANS_CLUSTERING_CLUSTER_H
