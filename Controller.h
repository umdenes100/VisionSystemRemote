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

    void onShowDestinationChanged(bool checked);
    void onShowObstaclesChanged(bool checked);
    void onShowCustomCoordinateChanged(bool checked);
    void onCustomXChanged(double x);
    void onCustomYChanged(double y);
    void onRandomize();

    void onApplySettings(int cameraDevice);
    void onFocusChanged(int focus);
    void onBrightnessChanged(int brightness);
    void onSharpnessChanged(int sharpness);

signals:

public slots:

private:
    Arena mArena;
    Camera mCamera;

    Server mServer;


};

#endif // CONTROLLER_H
