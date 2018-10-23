#ifndef ARUCOMARKER_H
#define ARUCOMARKER_H

typedef struct ArucoMarker {
    ArucoMarker(int id, int x1, int x2, int y1, int y2) {
        this->id = id;
        this->x[0] = x1;
        this->x[1] = x2;
        this->y[0] = y1;
        this->y[1] = y2;
    }


    int id;
    int x[2];
    int y[2];
} ArucoMarker;

#endif // ARUCOMARKER_H
