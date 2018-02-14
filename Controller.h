#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "Arena.h"
#include "Camera.h"
#include "Server.h"
#include "SerialPortList.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = 0);
    void start();

signals:

public slots:

private:
    Arena mArena;
    Camera mCamera;
    Server mServer;
    SerialPortList mSerialPortList;

};

#endif // CONTROLLER_H
