#include "Arena.h"

#include <stdlib.h>
#include <opencv2/imgproc.hpp>
#include <QMutexLocker>
#include <QDebug>

const float PI = 3.141592653589;
const float TARGET_DIAMETER = 0.18;
const float THRESHOLD = 0.1;
const float OBSTACLE_WIDTH = 0.2;
const float OBSTACLE_HEIGHT = 0.5;
const float OSV_WIDTH = 0.35;

Arena::Arena()
{
    mOriginPx[0] = 500;
    mOriginPx[1] = 500;
    mXAxisPx[0] = 600;
    mXAxisPx[1] = 600;
    mObstacles[0] = {2.0, 2.0, 1.0, 1.0};
    mObstacles[1] = {3.0, 1.5, 0.8, 0.2};
    mObstacles[2] = {1.5, 3.2, 1.2, 1.4};
//    mPpm = 1;
    mWidthM = 4;
    mHeightM = 2;
}

// Converts arena coordinates into pixel coordinates
cv::Point Arena::cameraCoordinate(float x, float y) {
    QMutexLocker lock(&mArenaMutex);
    // Convert coordinates to pixels
    float A = x * mPpm;
    float B = y * mPpm;
    
    // Transform into the arena
    float fx = A * cos(mTheta) - B * sin(mTheta);
    float fy = A * sin(mTheta) + B * cos(mTheta);
    
    return cv::Point(mOriginPx[0] + fx, mOriginPx[1] - fy);
}

// Draws the starting position, obstacles, and destination
void Arena::draw(cv::Mat& image) {
    mObstaclesMutex.lock();
    if (mDrawObstacles) {
        for (int i = 1; i < 3; i++) {
            drawRectangle(image, mObstacles[i].x, mObstacles[i].y, mObstacles[i].width, mObstacles[i].height);
        }
    }
    mObstaclesMutex.unlock();
    mDestinationMutex.lock();
    if (mDrawDestination) {
        drawCircle(image, mTargetLocation.x, mTargetLocation.y, TARGET_DIAMETER / 2);
    }
    mDestinationMutex.unlock();
    mCustomMutex.lock();
    if(mDrawCustom) {
        drawCircle(image, mCustomCoordinate.x, mCustomCoordinate.y, 0.09);
    }
    mCustomMutex.unlock();
    drawRectangle(image, mStartingLocation.x - 0.175, mStartingLocation.y + 0.175, 0.35, 0.35);

    Position pt1(mStartingLocation.x - 0.1 * cos(mStartingLocation.theta),
                    mStartingLocation.y - 0.1 * sin(mStartingLocation.theta), 0);
    Position pt2(mStartingLocation.x + 0.1 * cos(mStartingLocation.theta),
                 mStartingLocation.y + 0.1 * sin(mStartingLocation.theta), 0);

    cv::arrowedLine(image, cameraCoordinate(pt1.x, pt1.y),
                    cameraCoordinate(pt2.x, pt2.y),
                    cv::Scalar(0, 255, 0),
                    3,
                    8,
                    0,
                    0.3);
}

// Draws a circle at an arena X,Y location
void Arena::drawCircle(cv::Mat& image, float x, float y, float radius) {
    cv::ellipse(image,          // Image to draw on
        cameraCoordinate(x, y), // Coordinates of center
        cv::Size(radius * mPpm, radius * mPpm),// Radius
        0,                      // Rotation
        0,                      // Arc start degree
        360,                    // Arc end degree
        cv::Scalar(255, 200, 0),  // Color (BGR)
        2                       // Line thickness
    );
}

// Draws a rectangle in the arena
// @params x, y are arena coordinate of upper left corner of rectangle
// @params width, height are in meters
void Arena::drawRectangle(cv::Mat& image, float x, float y, float width, float height) {
    std::vector<cv::Point> points(4);

    points[0] = cameraCoordinate(x, y);
    points[1] = cameraCoordinate(x + width, y);
    points[2] = cameraCoordinate(x + width, y - height);
    points[3] = cameraCoordinate(x, y - height);

    cv::Mat contours = cv::Mat(points);
    cv::polylines(image,
        contours,
        true,
        cv::Scalar(0, 255, 0),
        2
    );
}

bool Arena::getPosition(int markerId, Marker& marker) {
    mMarkersMutex.lock();

    if(mMarkers.contains(markerId)){
        marker = mMarkers.value(markerId, Marker(markerId));
        mMarkersMutex.unlock();
        return true;
    }
    mMarkersMutex.unlock();
    return false;
}

// Translates and stores a set of detected markers
void Arena::processMarkers(cv::Mat& image, std::vector<ArucoMarker>& markers) {
    mMarkersMutex.lock();
    mMarkers.clear();

    foreach (ArucoMarker marker, markers) {
        //marker.draw(image, cv::Scalar(0, 0, 255), 2);

        if (marker.id == 0) {
            mArenaMutex.lock();
            //mOriginPx[0] = marker[0].x;
            mOriginPx[0] = marker.x[0];
            //mOriginPx[1] = marker[0].y;
            mOriginPx[1] = marker.y[0];
            mArenaMutex.unlock();
        } else if (marker.id == 1) {
            mArenaMutex.lock();
            //mXAxisPx[0] = marker[0].x;
            mXAxisPx[0] = marker.x[0];
            //mXAxisPx[1] = marker[0].y;
            mXAxisPx[1] = marker.y[0];
            mArenaMutex.unlock();
        } else {
            Marker temp = translate(marker);
            mMarkers.insert(marker.id, temp);

            // Draw an arrow indicating the orientation
            cv::arrowedLine(image,
                //cv::Point(marker[0].x, marker[0].y),
                //cv::Point(marker[1].x, marker[1].y),
                cv::Point(marker.x[0], marker.y[0]),
                cv::Point(marker.x[1], marker.y[1]),
                cv::Scalar(0, 255, 0),
                3,
                8,
                0,
                0.5
            );
        }
    }

    mMarkersMutex.unlock();

    mArenaMutex.lock();
    // Calculate pixels per meter for the frame
    mPpm = sqrt((mXAxisPx[0] - mOriginPx[0])*(mXAxisPx[0] - mOriginPx[0]) +
        (mXAxisPx[1] - mOriginPx[1]) * (mXAxisPx[1] - mOriginPx[1])) / mWidthM;
    // Calculate the tilt of the arena in this frame
    mTheta = -atan2(mXAxisPx[1] - mOriginPx[1], mXAxisPx[0] - mOriginPx[0]);
    mArenaMutex.unlock();
}

// Randomizes mission variables
void Arena::randomize() {

    // Generate a random starting position
    mStartingLocation.x = 0.35;
    mStartingLocation.y = 0.4 + (rand() % 5) * 0.3;
    mStartingLocation.theta = (rand() % 4) * PI / 2 - PI;

    static const int presets[6][3] = {
        {0, 1, 2},
        {2, 1, 0},
        {0, 2, 1},
        {2, 0, 1},
        {1, 0, 2},
        {1, 2, 0}
    };

    const int randomization = rand() % 6;

    // now we hav selected the randomization we now need to randomize the obstacles accordingly
    // Generate random positions and orientations for obstacles
    mDestinationMutex.lock();
    mObstaclesMutex.lock();

    // Akhil's Algorithm

    for(int i = 0; i < 3; i++) {
        // y location is either 1.8, 1.25, or 0.7: distances of 0.65
        float baseY = presets[randomization][i] * 0.65 + OBSTACLE_HEIGHT + 0.1;
        mObstacles[i].y = baseY;

        mObstacles[i].x = i * 0.55 + 1.5;
        mObstacles[i].height = OBSTACLE_HEIGHT;
        mObstacles[i].width = OBSTACLE_WIDTH;
    }

    // the target randomization will be in a box
    float xMin = 2.8 + 0.4 + TARGET_DIAMETER / 2;
    float xMax = 4 - OSV_WIDTH - 0.1 - TARGET_DIAMETER / 2;
    float yMin = 0.4 + TARGET_DIAMETER / 2;
    float yMax = 2 - 0.4 - TARGET_DIAMETER / 2;

    // we now have ranges
    mTargetLocation.x = (rand() % 100) / 100.0 * (xMax - xMin) + xMin;
    mTargetLocation.y = (rand() % 100) / 100.0 * (yMax - yMin) + yMin;

    mObstaclesMutex.unlock();
    mDestinationMutex.unlock();
}

void Arena::setSize(float width, float height) {
    mArenaMutex.lock();
    mWidthM = width;
    mHeightM = height;
    mArenaMutex.unlock();
}

// Translates an Aruco Marker into a VS Marker and highlights in the image
Marker Arena::translate(ArucoMarker m) {
    mArenaMutex.lock();
    // Calculate theta of the marker by comparing the degree of the line created
    // by two corners with the degree of the arena
    //float theta = mTheta - atan2(m[1].y - m[0].y, m[1].x - m[0].x);
    float theta = mTheta - atan2(m.y[1] - m.y[0], m.x[1] - m.x[0]);


    // Subtract away the origin
    //float fx = m[0].x - mOriginPx[0];
    //float fy = mOriginPx[1] - m[0].y;
    float fx = m.x[0] - mOriginPx[0];
    float fy = mOriginPx[1] - m.y[0];

    // Convert camera frame of reference to arena frame of reference
    float A = fx * cos(mTheta) + fy * sin(mTheta);
    float B = fy * cos(mTheta) - fx * sin(mTheta);

    // Shift measurement to center of marker
    //float markerSide = sqrt((m[1].x - m[0].x)*(m[1].x - m[0].x) + (m[1].y - m[0].y)*(m[1].y - m[0].y));
    float markerSide = sqrt((m.x[1] - m.x[0])*(m.x[1] - m.x[0]) + (m.y[1] - m.y[0])*(m.y[1] - m.y[0]));

    if (cos(theta) >= 0) {
        A += sqrt(2) * markerSide / 2 * cos(PI/4 - theta);
        B -= sqrt(2) * markerSide / 2 * sin(PI/4 - theta);
    } else {
        A -= sqrt(2) * markerSide / 2 * sin(theta - 3*PI/4);
        B += sqrt(2) * markerSide / 2 * cos(theta - 3*PI/4);
    }

    // Convert to meters and store into Marker
    float x = A / mPpm;
    float y = B / mPpm;

    mArenaMutex.unlock();

    Marker marker = Marker(m.id, x, y, theta);


    return marker;
}

Position Arena::getTargetLocation(){
    QMutexLocker lock(&mDestinationMutex);
    return mTargetLocation;
}

void Arena::onCustomXChanged(double x) {
    mCustomMutex.lock();
    mCustomCoordinate.x = static_cast<float>(x);
    mCustomMutex.unlock();
}

void Arena::onCustomYChanged(double y) {
    mCustomMutex.lock();
    mCustomCoordinate.y = static_cast<float>(y);
    mCustomMutex.unlock();
}

void Arena::onDrawCustomChanged(bool draw) {
    mCustomMutex.lock();
    mDrawCustom = draw;
    mCustomMutex.unlock();
}

void Arena::onDrawDestinationChanged(bool draw) {
    mDestinationMutex.lock();
    mDrawDestination = draw;
    mDestinationMutex.unlock();
}

void Arena::onDrawObstaclesChanged(bool draw) {
    mObstaclesMutex.lock();
    mDrawObstacles = draw;
    mObstaclesMutex.unlock();
}

inline float Arena::max(float a, float b) {
    return a > b ? a : b;
}

inline float Arena::min(float a, float b) {
    return a < b ? a : b;
}
