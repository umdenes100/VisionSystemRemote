#include <opencv2/imgproc.hpp>

#ifndef OBSTACLE_H
#define OBSTACLE_H

typedef struct Obstacle {
    float x;
    float y;

    float width;
    float height;
    cv::Scalar color;
} Obstacle;

#endif /* OBSTACLE_H */
