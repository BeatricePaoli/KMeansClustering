#include <cmath>
#include "Point.h"

float Point::dist(const Point &p) const {
    float sum = (this->x - p.x) * (this->x - p.x) + (this->y - p.y) * (this->y - p.y);
    return sqrtf(sum);
}
