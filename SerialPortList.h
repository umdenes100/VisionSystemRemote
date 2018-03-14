#ifndef SERIALPORTLIST_H
#define SERIALPORTLIST_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QMutex>

#include "Arena.h"
#include "SerialPort.h"

class SerialPortList : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortList(Arena& arena, QObject *parent = 0);

    QMap<QString, SerialPort*>& getMap();
    QMutex mSerialPortsMutex;

signals:
    void newSerialPort();
    void newCommand(QString portName, CommandType type, QString message);
    void newMessage(QString portName, QString message);
    void newName();
    void newSerialPort(QString portName);

private slots:
    void onError(QSerialPort::SerialPortError error);
    void onNewCommand(QString portName, CommandType type, QString message);
    void onNewMessage(QString portName, QString message);
    void onNewName();
    void refreshPorts();

private:
    Arena& mArena;
    QTimer mRefreshPortsTimer;
    QMap<QString, SerialPort*> mSerialPorts;

};

#endif // SERIALPORTLIST_H
