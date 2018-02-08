#ifndef MARKER_H
#define MARKER_H

#include "Position.h"

typedef struct Marker : public Position {
    Marker(int id):
        Position(),
        id(id)
    {};
    Marker(int id, float x, float y, float theta):
        Position(x, y, theta),
        id(id)
    {};
    int id;
} Marker;

#endif // MARKER_H
