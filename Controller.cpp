#include <QThread>

#include "Controller.h"

Controller::Controller(QObject *parent) : QObject(parent),
    mArena()
{
    mCamera = new Camera(mArena);
    mCamera->moveToThread(&mCameraThread);
    connect(&mCameraThread, SIGNAL(finished()), mCamera, SLOT(deleteLater()));
    connect(&mCameraThread, SIGNAL(finished()), &mCameraThread, SLOT(deleteLater()));

    mConnectionList = new ConnectionList(mArena);
    mConnectionList->moveToThread(&mConnectionListThread);
    connect(&mConnectionListThread, SIGNAL(finished()), mConnectionList, SLOT(deleteLater()));
    connect(&mConnectionListThread, SIGNAL(finished()), &mConnectionListThread, SLOT(deleteLater()));
    
    mServer = new Server(mConnectionList);
    connect(mCamera, SIGNAL(newFrame(QImage)), mServer, SLOT(onNewFrame(QImage)));
    connect(mConnectionList, SIGNAL(newMessage(QString,QString)), mServer, SLOT(onNewMessage(QString,QString)));
    connect(mConnectionList, SIGNAL(newConnection(QString)), mServer, SLOT(addNameToMap(QString)));
    connect(mConnectionList, SIGNAL(newName()), mServer, SLOT(onNewName()));
    connect(mConnectionList, SIGNAL(newCommand(QString,QString,QString)), mServer, SLOT(onNewCommand(QString,QString,QString)));
    connect(&mServerThread, SIGNAL(finished()), mServer, SLOT(deleteLater()));
    connect(&mServerThread, SIGNAL(finished()), &mServerThread, SLOT(deleteLater()));
    connect(&mServerThread, SIGNAL(started()), mServer, SLOT(start()));
}

Controller::~Controller(){
    mCameraThread.quit();
    mConnectionListThread.quit();
    mServerThread.quit();
}

void Controller::start() {
    mCameraThread.start();
    mConnectionListThread.start();
    mServerThread.start();
    mCamera->applySettings(0, QSize(1920, 1080), 15, 0.1);
}

void Controller::onShowDestinationChanged(bool checked)
{
    mArena.onDrawDestinationChanged(checked);
}

void Controller::onShowObstaclesChanged(bool checked)
{
    mArena.onDrawObstaclesChanged(checked);
}

void Controller::onShowCustomCoordinateChanged(bool checked)
{
    mArena.onDrawCustomChanged(checked);
}

void Controller::onCustomXChanged(double x)
{
    mArena.onCustomXChanged(x);
}

void Controller::onCustomYChanged(double y)
{
    mArena.onCustomYChanged(y);
}


Position Controller::onRandomize()
{
    mArena.randomize();
    return mArena.getTargetLocation();
}

void Controller::onApplySettings(int cameraDevice)
{
    mCamera->applySettings(cameraDevice, QSize(1920, 1080), 15, 0.1);
}

void Controller::onFocusChanged(int focus)
{
    mCamera->onFocusChanged(focus);
}

void Controller::onBrightnessChanged(int brightness)
{
    mCamera->onBrightnessChanged(brightness);
}

void Controller::onSharpnessChanged(int sharpness)
{
    mCamera->onSharpnessChanged(sharpness);
}
