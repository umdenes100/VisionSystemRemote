#include "Controller.h"

Controller::Controller(QObject *parent) : QObject(parent),
    mArena(),
    mCamera(mArena),
    mServer(),
    mSerialPortList(mArena, mServer)
{

}

void Controller::start() {

}
