#include "Controller.h"

Controller::Controller(QObject *parent) : QObject(parent),
    mArena(),
    mCamera(mArena),
    mServer(mArena)
{
    connect(&mCamera, SIGNAL(newFrame(QImage)), &mServer, SLOT(onNewFrame(QImage)));
}

void Controller::start() {
    mCamera.applySettings(1, QSize(1920, 1080), 15, 0.1);
}

void Controller::onShowDestinationChanged(bool checked)
{
    mCamera.onDrawDestinationChanged(checked);
}

void Controller::onShowObstaclesChanged(bool checked)
{
    mCamera.onDrawObstaclesChanged(checked);
}

void Controller::onShowCustomCoordinateChanged(bool checked)
{
    mCamera.onDrawCustomChanged(checked);
}

void Controller::onCustomXChanged(double x)
{
    mCamera.onCustomXChanged(x);
}

void Controller::onCustomYChanged(double y)
{
    mCamera.onCustomXChanged(y);
}


void Controller::onRandomize()
{
    mArena.randomize();
}

void Controller::onApplySettings(int cameraDevice)
{
    mCamera.applySettings(cameraDevice, QSize(1920, 1080), 15, 0.1);
}

void Controller::onFocusChanged(int focus)
{
    mCamera.onFocusChanged(focus);
}

void Controller::onBrightnessChanged(int brightness)
{
    mCamera.onBrightnessChanged(brightness);
}

void Controller::onSharpnessChanged(int sharpness)
{
    mCamera.onSharpnessChanged(sharpness);
}
