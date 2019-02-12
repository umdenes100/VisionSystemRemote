#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <QObject>
#include <QTimer>
#include <QImage>

#include "Arena.h"

class Camera : public QObject {
    Q_OBJECT

public:
    explicit Camera(Arena& arena);
    void resetCamera();
    bool isBestekerCamera;

public slots:
    void applySettings(uint cameraDevice, QSize resolution, uint frameRate, float markerSize);
    void onBrightnessChanged(int brightness);
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
    //cv::aruco::MarkerDetector mMarkerDetector;
    //cv::aruco::CameraParameters mCameraParameters;

    float mMarkerSize;

    uint mCameraDevice;
    uint mBrightness;
    uint mFocus;
    uint mSharpness;

    Arena& mArena;
};

#endif // CAMERA_H
