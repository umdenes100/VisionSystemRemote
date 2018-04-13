#include "SerialPortList.h"

#include <QSerialPortInfo>
#include <QWebSocket>
#include <QMutexLocker>
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
    return mSerialPorts;
}

void SerialPortList::refreshPorts() {
    mSerialPortsMutex.lock();
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    foreach (QSerialPortInfo port, availablePorts) {
        if (!mSerialPorts.contains(port.portName())) {
            SerialPort *serialPort = new SerialPort(port, mArena);
            connect(serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(onError(QSerialPort::SerialPortError)));
            connect(serialPort, SIGNAL(newMessage(QString, QString)), this, SLOT(onNewMessage(QString, QString)));
            connect(serialPort, SIGNAL(newName()), this, SLOT(onNewName()));
            connect(serialPort, SIGNAL(newCommand(QString,int,QString)), SLOT(onNewCommand(QString,int,QString)));
            mSerialPorts.insert(port.portName(), serialPort);
            emit newSerialPort(port.portName());
        }
    }
    mSerialPortsMutex.unlock();
}

//NOTE: if devices close unexpectedly, the proper error condition should be checked
void SerialPortList::onError(QSerialPort::SerialPortError error) {
    mSerialPortsMutex.lock();
    QObject *sender = QObject::sender();
    qDebug() << sender;
    if (sender != NULL) {
        SerialPort *serialPort = static_cast<SerialPort*>(sender);
        qDebug() << "Closing port " << serialPort->portName();
        mSerialPorts.remove(serialPort->portName());
        delete serialPort;
    }
    mSerialPortsMutex.unlock();
}

void SerialPortList::onNewMessage(QString portName, QString message){
    emit newMessage(portName, message);
}

void SerialPortList::onNewName() {
    emit newName();
}

void SerialPortList::onNewCommand(QString portName, int type, QString message) {
    // qDebug() << portName << ": " << message;
    emit newCommand(portName, type, message);
}
