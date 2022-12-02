#include "Cluster.h"

Cluster::Cluster(int idVal, Point centr) {
    this->id = idVal;
    this->centroid = centr;
    this->tempSumX = 0;
    this->tempSumY = 0;
}

void Cluster::addPoint(Point *point) {
    this->points.push_back(*point);
    this->tempSumX += point->x;
    this->tempSumY += point->y;
}

void Cluster::updateCentroid() {
    int clusterSize = this->points.size();
    this->centroid.x = this->tempSumX / clusterSize;
    this->centroid.y = this->tempSumY / clusterSize;
    this->tempSumX = 0;
    this->tempSumY = 0;
}
