#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <aruco/aruco.h>
#include <QObject>
#include <QTimer>
#include <QImage>

#include "Arena.h"

class Camera : public QObject {
    Q_OBJECT

public:
    explicit Camera(Arena& arena);

public slots:
    void applySettings(uint cameraDevice, QSize resolution, uint frameRate, float markerSize);
    void onBrightnessChanged(int brightness);
    void onCustomXChanged(double x);
    void onCustomYChanged(double y);
    void onDrawCustomChanged(bool draw);
    void onDrawDestinationChanged(bool draw);
    void onDrawObstaclesChanged(bool draw);
    void onFocusChanged(int focus);
    void onSharpnessChanged(int sharpness);

signals:
    void newFrame(QImage image);

private slots:
    void capture();

private:
    inline QImage cvMatToQImage(const cv::Mat& inMat);

    QTimer mCaptureTimer;

    cv::VideoCapture mVideoCapture;
    aruco::MarkerDetector mMarkerDetector;
    aruco::CameraParameters mCameraParameters;

    float mMarkerSize;

    Arena& mArena;

    bool mDrawCustom;
    bool mDrawObstacles;
    bool mDrawDestination;

    Position mCustomCoordinate;

    uint mCameraDevice;
    uint mBrightness;
    uint mFocus;
    uint mSharpness;
};

#endif // CAMERA_H
