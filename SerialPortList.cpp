#include "SerialPortList.h"

#include <QSerialPortInfo>
#include <QDebug>

SerialPortList::SerialPortList(Arena& arena, Server& server, QObject *parent) :
    QObject(parent),
    mArena(arena),
    mServer(server),
    mRefreshPortsTimer()
{
    connect(&mRefreshPortsTimer, SIGNAL(timeout()), SLOT(refreshPorts()));
    mRefreshPortsTimer.start(500);
}

void SerialPortList::refreshPorts() {
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    foreach (QSerialPortInfo port, availablePorts) {
        if (!mSerialPorts.contains(port.portName())) {
            SerialPort *serialPort = new SerialPort(port, mArena);
            connect(serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(onError(QSerialPort::SerialPortError)));
            connect(serialPort, SIGNAL(transmit(QString)), &mServer, SLOT(onNewMessage(QString)));
            mSerialPorts.insert(port.portName(), serialPort);
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
