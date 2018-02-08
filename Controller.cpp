#include "Controller.h"

Controller::Controller(QObject *parent) : QObject(parent),
    mArena(),
    mCamera(mArena),
//    mSerialPortList(),
    mServer()
{

}

void Controller::start() {

}
