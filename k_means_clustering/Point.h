#ifndef K_MEANS_CLUSTERING_POINT_H
#define K_MEANS_CLUSTERING_POINT_H

#include <vector>

class Point {
public:
    Point(): x(0), y(0), clusterId(-1) {};
    Point(float x, float y): x(x), y(y), clusterId(-1) {};

    float dist(const Point &p) const;

    float x;
    float y;
    int clusterId;
};

#endif //K_MEANS_CLUSTERING_POINT_H
