#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "Arena.h"
#include "Camera.h"
#include "Server.h"

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
//    SerialPortList mSerialPortList;
    Server mServer;
};

#endif // CONTROLLER_H
