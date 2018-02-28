#ifndef SERIALPORTLIST_H
#define SERIALPORTLIST_H

#include <QObject>
#include <QTimer>
#include <QMap>

#include "Arena.h"
#include "SerialPort.h"

class SerialPortList : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortList(Arena& arena, QObject *parent = 0);

    QMap<QString, SerialPort*>& getMap();

signals:
    void newSerialPort();
    void newMessage(QString portName, QString message);
    void newName();
    void newSerialPort(QString portName);

private slots:
    void onError(QSerialPort::SerialPortError error);
    void onNewMessage(QString portName, QString message);
    void onNewName();
    void refreshPorts();

private:
    Arena& mArena;
    QTimer mRefreshPortsTimer;
    QMap<QString, SerialPort*> mSerialPorts;

};

#endif // SERIALPORTLIST_H
