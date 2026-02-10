#ifndef POSITION_H
#define POSITION_H

#include "Vector.h"

class Position2D : public GVector::Vector2D {
public:
    Position2D(double x, double y) : GVector::Vector2D(x, y) {}
};

#endif // POSITION_H
