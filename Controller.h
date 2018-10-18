#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>

#include "Arena.h"
#include "Camera.h"
#include "Server.h"
#include "SerialPortList.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = 0);
    ~Controller();
    void start();
    Camera* mCamera;

    void onShowDestinationChanged(bool checked);
    void onShowObstaclesChanged(bool checked);
    void onShowCustomCoordinateChanged(bool checked);
    void onCustomXChanged(double x);
    void onCustomYChanged(double y);
    Position onRandomize();

    void onApplySettings(int cameraDevice);
    void onFocusChanged(int focus);
    void onBrightnessChanged(int brightness);
    void onSharpnessChanged(int sharpness);

signals:

public slots:

private:
    Arena mArena;

    SerialPortList* mSerialPortList;
    Server* mServer;

    QThread mCameraThread;
    QThread mSerialPortListThread;
    QThread mServerThread;
};

#endif // CONTROLLER_H
