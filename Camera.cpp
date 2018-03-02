#include "Camera.h"

#include <QDebug>

Camera::Camera(Arena& arena) : QObject(), mArena(arena) {
    connect(&mCaptureTimer, SIGNAL(timeout()), SLOT(capture()));
    mMarkerDetector.setDetectionMode(aruco::DM_VIDEO_FAST, 0.001);
}

void Camera::capture() {
    static cv::Mat image;
    static std::vector<aruco::Marker> markers;

    if (mVideoCapture.isOpened() && mVideoCapture.grab()) {
        mVideoCapture.retrieve(image);

        mMarkerDetector.detect(image, markers, mCameraParameters, mMarkerSize);
        mArena.processMarkers(image, markers);
        mArena.draw(image, mDrawObstacles, mDrawDestination);

        if (mDrawCustom) {
            mArena.drawCircle(image, mCustomCoordinate.x, mCustomCoordinate.y, 0.09);
        }

        emit newFrame(cvMatToQImage(image));
    }
}

void Camera::applySettings(uint cameraDevice, QSize resolution, uint frameRate, float markerSize) {
    mCameraDevice = cameraDevice;
    mVideoCapture.open(cameraDevice);

    mVideoCapture.set(cv::CAP_PROP_FOURCC, CV_FOURCC('M','J','P','G'));
    mVideoCapture.set(cv::CAP_PROP_FRAME_WIDTH, resolution.width());
    mVideoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.height());
    mCaptureTimer.start(frameRate);

    mMarkerSize = markerSize;
}

void Camera::onBrightnessChanged(int brightness) {
    QString command;
    command = command + QString("v4l2-ctl -d /dev/video");
    command = command + QString::number(mCameraDevice);
    command = command + QString(" -c brightness=");
    command = command + QString::number(brightness);

    system(command.toStdString().c_str());
}

void Camera::onCustomXChanged(double x) {
    mCustomCoordinate.x = static_cast<float>(x);
}

void Camera::onCustomYChanged(double y) {
    mCustomCoordinate.y = static_cast<float>(y);
}

void Camera::onDrawCustomChanged(bool draw) {
    mDrawCustom = draw;
}

void Camera::onDrawDestinationChanged(bool draw) {
    mDrawDestination = draw;
}

void Camera::onDrawObstaclesChanged(bool draw) {
    mDrawObstacles = draw;
}

void Camera::onFocusChanged(int focus) {
    QString command;
    command = command + QString("v4l2-ctl -d /dev/video");
    command = command + QString::number(mCameraDevice);
    command = command + QString(" -c focus_absolute=");
    command = command + QString::number(focus);

    system(command.toStdString().c_str());
}

void Camera::onSharpnessChanged(int sharpness) {
    QString command;
    command = command + QString("v4l2-ctl -d /dev/video");
    command = command + QString::number(mCameraDevice);
    command = command + QString(" -c sharpness=");
    command = command + QString::number(sharpness);

    system(command.toStdString().c_str());
}

inline QImage Camera::cvMatToQImage(const cv::Mat& inMat)
{
    switch (inMat.type())
    {
        // 8-bit, 4 channel
        case CV_8UC4:
        {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );
            return image;
        }
        // 8-bit, 3 channel
        case CV_8UC3:
        {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );
            return image.rgbSwapped();
        }
        // 8-bit, 1 channel
        case CV_8UC1:
        {
            static QVector<QRgb>  sColorTable;
            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
            for ( int i = 0; i < 256; ++i )
                sColorTable.push_back( qRgb( i, i, i ) );
            }
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );
            image.setColorTable( sColorTable );
            return image;
        }
        default:
            //qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
    }

    return QImage();
}
