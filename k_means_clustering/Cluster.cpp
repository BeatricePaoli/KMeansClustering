#include "Cluster.h"

Cluster::Cluster(int idVal, Point centr) {
    this->id = idVal;
    this->centroid = centr;
    this->tempSumX = 0;
    this->tempSumY = 0;
    this->size = 0;
}

void Cluster::addPoint(Point point) {
#pragma omp atomic
    this->tempSumX += point.x;
#pragma omp atomic
    this->tempSumY += point.y;
#pragma omp atomic
    this->size++;
}

void Cluster::updateCentroid() {
    this->centroid.x = this->tempSumX / this->size;
    this->centroid.y = this->tempSumY / this->size;
    this->tempSumX = 0;
    this->tempSumY = 0;
    this->size = 0;
}
