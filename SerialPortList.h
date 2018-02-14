#ifndef SERIALPORTLIST_H
#define SERIALPORTLIST_H

#include <QObject>
#include <QTimer>
#include <QMap>

#include "Arena.h"
#include "SerialPort.h"
#include "Server.h"

class SerialPortList : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortList(Arena& arena, Server& server, QObject *parent = 0);

private slots:
    void onError(QSerialPort::SerialPortError error);
    void refreshPorts();

private:
    Arena& mArena;
    Server& mServer;
    QTimer mRefreshPortsTimer;
    QMap<QString, SerialPort*> mSerialPorts;

};

#endif // SERIALPORTLIST_H
