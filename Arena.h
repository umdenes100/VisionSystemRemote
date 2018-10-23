#ifndef ARENA_H
#define ARENA_H

#include <QMap>
#include <QMutex>
#include <opencv2/core.hpp>
#include <vector>
//#include <aruco/aruco.h>
#include <opencv2/aruco.hpp>
#include "arucomarker.h"
#include "Marker.h"
#include "Position.h"
#include "Obstacle.h"
#include "Marker.h"

class Arena {
public:
    explicit Arena();
    cv::Point cameraCoordinate(float x, float y);
    void draw(cv::Mat& image);
    void drawCircle(cv::Mat& image, float x, float y, float radius);
    void drawRectangle(cv::Mat& image, float x, float y, float width, float height);
    bool getPosition(int markerId, Marker& marker);
    Position getTargetLocation();
    //void processMarkers(cv::Mat& image, std::vector<aruco::Marker>& markers);
    void processMarkers(cv::Mat& image, std::vector<ArucoMarker>& markers);
    void randomize();
    void setSize(float width, float height);
    //Marker translate(aruco::Marker m);
    Marker translate(ArucoMarker m);


    void onCustomXChanged(double x);
    void onCustomYChanged(double y);
    void onDrawCustomChanged(bool draw);
    void onDrawDestinationChanged(bool draw);
    void onDrawObstaclesChanged(bool draw);

private:
    int mOriginPx[2];
    int mXAxisPx[2];

    QMutex mArenaMutex;
    float mWidthM;
    float mHeightM;
    float mPpm;
    float mTheta;

    Position mStartingLocation;
    Obstacle mObstacles[3];
    Position mTargetLocation;

    QMutex mMarkersMutex;
    QMap<int, Marker> mMarkers;

    inline float max(float a, float b);
    inline float min(float a, float b);

    QMutex mCustomMutex;
    bool mDrawCustom;
    QMutex mObstaclesMutex;
    bool mDrawObstacles;
    QMutex mDestinationMutex;
    bool mDrawDestination;

    Position mCustomCoordinate;
};

#endif /* ARENA_H */
