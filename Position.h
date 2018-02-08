#ifndef POSITION_H
#define POSITION_H

typedef struct Position {
    Position():
        x(0.0),
        y(0.0),
        theta(0.0)
    {};
    Position(float x, float y, float theta):
        x(x),
        y(y),
        theta(theta)
    {};
    float x;
    float y;
    float theta;
} Position;

#endif /* POSITION_H */
