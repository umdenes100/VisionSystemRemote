#include "SerialPortList.h"

#include <QSerialPortInfo>
#include <QWebSocket>
#include <QDebug>

SerialPortList::SerialPortList(Arena& arena, QObject *parent) :
    QObject(parent),
    mArena(arena),
    mRefreshPortsTimer()
{
    connect(&mRefreshPortsTimer, SIGNAL(timeout()), SLOT(refreshPorts()));
    mRefreshPortsTimer.start(500);
}

QMap<QString, SerialPort *>& SerialPortList::getMap(){
    refreshPorts();
    return mSerialPorts;
}

void SerialPortList::refreshPorts() {
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    foreach (QSerialPortInfo port, availablePorts) {
        if (!mSerialPorts.contains(port.portName())) {
            SerialPort *serialPort = new SerialPort(port, mArena);
            connect(serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(onError(QSerialPort::SerialPortError)));
            connect(serialPort, SIGNAL(newMessage(QString, QString)), this, SLOT(onNewMessage(QString, QString)));
            connect(serialPort, SIGNAL(newName()), this, SLOT(onNewName()));
            connect(serialPort, SIGNAL(newCommand(QString,CommandType,QString)), SLOT(onNewCommand(QString,CommandType,QString)));
            mSerialPorts.insert(port.portName(), serialPort);
            emit newSerialPort(port.portName());
        }
    }
}

//NOTE: if devices close unexpectedly, the proper error condition should be checked
void SerialPortList::onError(QSerialPort::SerialPortError error) {
    QObject *sender = QObject::sender();
    qDebug() << sender;
    if (sender != NULL) {
        SerialPort *serialPort = static_cast<SerialPort*>(sender);
        qDebug() << "Closing port " << serialPort->portName();
        mSerialPorts.remove(serialPort->portName());
        delete serialPort;
    }
}

void SerialPortList::onNewMessage(QString portName, QString message){
    emit newMessage(portName, message);
}

void SerialPortList::onNewName() {
    emit newName();
}

void SerialPortList::onNewCommand(QString portName, CommandType type, QString message) {
    emit newCommand(portName, type, message);
}
