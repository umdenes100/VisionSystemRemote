#include "Controller.h"

Controller::Controller(QObject *parent) : QObject(parent),
    mArena(),
    mCamera(mArena),
    mServer(mArena)
{
    connect(&mCamera, SIGNAL(newFrame(QImage)), &mServer, SLOT(onNewFrame(QImage)));
}

void Controller::start() {
    mCamera.applySettings(0, QSize(1920, 1080), 15, 0.1);
}
