#include <QThread>

#include "Controller.h"

Controller::Controller(QObject *parent) : QObject(parent),
    mArena()
{
    mCamera = new Camera(mArena);
    mCamera->moveToThread(&mCameraThread);
    connect(&mCameraThread, SIGNAL(finished()), mCamera, SLOT(deleteLater()));
    connect(&mCameraThread, SIGNAL(finished()), &mCameraThread, SLOT(deleteLater()));

    mSerialPortList = new SerialPortList(mArena);
    mSerialPortList->moveToThread(&mSerialPortListThread);
    connect(&mSerialPortListThread, SIGNAL(finished()), mSerialPortList, SLOT(deleteLater()));
    connect(&mSerialPortListThread, SIGNAL(finished()), &mSerialPortListThread, SLOT(deleteLater()));
    
    mServer = new Server(mSerialPortList);
    connect(mCamera, SIGNAL(newFrame(QImage)), mServer, SLOT(onNewFrame(QImage)));
    connect(mSerialPortList, SIGNAL(newMessage(QString,QString)), mServer, SLOT(onNewMessage(QString,QString)));
    connect(mSerialPortList, SIGNAL(newSerialPort(QString)), mServer, SLOT(addNameToMap(QString)));
    connect(mSerialPortList, SIGNAL(newName()), mServer, SLOT(onNewName()));
    connect(mSerialPortList, SIGNAL(newCommand(QString,int,QString)), mServer, SLOT(onNewCommand(QString,int,QString)));
    connect(&mServerThread, SIGNAL(finished()), mServer, SLOT(deleteLater()));
    connect(&mServerThread, SIGNAL(finished()), &mServerThread, SLOT(deleteLater()));
    connect(&mServerThread, SIGNAL(started()), mServer, SLOT(start()));
}

Controller::~Controller(){
    mCameraThread.quit();
    mSerialPortListThread.quit();
    mServerThread.quit();
}

void Controller::start() {
    mCameraThread.start();
    mSerialPortListThread.start();
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
    mArena.onCustomXChanged(y);
}


void Controller::onRandomize()
{
    mArena.randomize();
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
