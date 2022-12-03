#include "Cluster.h"

Cluster::Cluster(int idVal, Point centr) {
    this->id = idVal;
    this->centroid = centr;
    this->tempSumX = 0;
    this->tempSumY = 0;
    this->size = 0;
}

void Cluster::addPoint(Point *point) {
    this->tempSumX += point->x;
    this->tempSumY += point->y;
    this->size++;
}

void Cluster::updateCentroid() {
    this->centroid.x = this->tempSumX / this->size;
    this->centroid.y = this->tempSumY / this->size;
    this->tempSumX = 0;
    this->tempSumY = 0;
    this->size = 0;
}
