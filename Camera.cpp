#include "Camera.h"
#include <QProcess>
#include <QDebug>

Camera::Camera(Arena& arena) : QObject(), mArena(arena) {
    connect(&mCaptureTimer, SIGNAL(timeout()), SLOT(capture()));
    mMarkerDetector.setDetectionMode(aruco::DM_VIDEO_FAST, 0.001);
    mArena.randomize();
}

void Camera::capture() {
    static cv::Mat image;
    static std::vector<aruco::Marker> markers;

    if (mVideoCapture.isOpened() && mVideoCapture.grab()) {
        mVideoCapture.retrieve(image);

        mMarkerDetector.detect(image, markers, mCameraParameters, mMarkerSize);
        mArena.processMarkers(image, markers);
        mArena.draw(image);

        emit newFrame(cvMatToQImage(image));
    }
}

void Camera::applySettings(uint cameraDevice, QSize resolution, uint frameRate, float markerSize) {
    mCameraDevice = cameraDevice;
    QString bestekerCamera = QString("USB  Live  Camera");//name of the Besteker camera
    QString command = QString("v4l2-ctl --list-devices");

    QProcess process;
    process.start(command);
    process.waitForFinished(-1);

    if (process.readAllStandardOutput().indexOf(bestekerCamera) != -1) {
        //check for whether a live camera is plugged in (Besteker)
        //Besteker has different brightness, sharpness settings and doesn't have a focus option
        isBestekerCamera = true;
    }
    mVideoCapture.open(cameraDevice);

    mVideoCapture.set(cv::CAP_PROP_FOURCC, CV_FOURCC('M','J','P','G'));
    mVideoCapture.set(cv::CAP_PROP_FRAME_WIDTH, resolution.width());
    mVideoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.height());
    mCaptureTimer.start(frameRate);

    mMarkerSize = markerSize;
}

void Camera::onFocusChanged(int focus) {
    QString command;

    if (isBestekerCamera) return;

    command = command + QString("v4l2-ctl -d /dev/video");
    command = command + QString::number(mCameraDevice);
    command = command + QString(" -c focus_absolute=");
    command = command + QString::number(focus);

    system(command.toStdString().c_str());
}

void Camera::onSharpnessChanged(int sharpness) {
    QString command;

    if (isBestekerCamera) {
        sharpness = sharpness * 6.0 / 255.0;
    }
    command = command + QString("v4l2-ctl -d /dev/video");
    command = command + QString::number(mCameraDevice);
    command = command + QString(" -c sharpness=");
    command = command + QString::number(sharpness);

    system(command.toStdString().c_str());
}

void Camera::onBrightnessChanged(int brightness) {
    QString command;
    if (isBestekerCamera) {
        brightness = brightness * 128.0 / 255.0 - 64;
    }
    command = command + QString("v4l2-ctl -d /dev/video");
    command = command + QString::number(mCameraDevice);
    command = command + QString(" -c brightness=");
    command = command + QString::number(brightness);

    system(command.toStdString().c_str());
}

void Camera::resetCamera() {
    //'v4l2-ctl -d /dev/video2 --all' lists default values for the camera
    int sharpness, brightness, contrast;
    QString command;
    if (isBestekerCamera) {
        sharpness = 6;
        contrast = 64;
        brightness = -64;
    } else {
        sharpness = 128;
        contrast = 128;
        brightness = 128;
    }

    //set sharpness
    command = command + QString("v4l2-ctl -d /dev/video");
    command = command + QString::number(mCameraDevice);
    command = command + QString(" -c sharpness=");
    command = command + QString::number(sharpness);
    system(command.toStdString().c_str());

    //set brightness
    command = QString("v4l2-ctl -d /dev/video");
    command = command + QString::number(mCameraDevice);
    command = command + QString(" -c brightness=");
    command = command + QString::number(brightness);
    system(command.toStdString().c_str());

    //set contrast
    command = QString("v4l2-ctl -d /dev/video");
    command = command + QString::number(mCameraDevice);
    command = command + QString(" -c contrast=");
    command = command + QString::number(contrast);
    system(command.toStdString().c_str());

    //set focus
    if (!isBestekerCamera) {
        command = QString("v4l2-ctl -d /dev/video");
        command = command + QString::number(mCameraDevice);
        command = command + QString(" -c focus_absolute=");
        command = command + QString::number(0);
        system(command.toStdString().c_str());
    }

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
