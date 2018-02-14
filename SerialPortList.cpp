#include "SerialPortList.h"

#include <QSerialPortInfo>

SerialPortList::SerialPortList(Arena& arena, QObject *parent) :
    QObject(parent),
    mArena(arena),
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
            connect(serialPort, SIGNAL(aboutToClose()), SLOT(onPortAboutToClose()));
            mSerialPorts.insert(port.portName(), serialPort);
        }
    }
}

void SerialPortList::onPortAboutToClose() {
    QObject *sender = QObject::sender();
    if (sender != NULL) {
        SerialPort serialPort = static_cast<SerialPort*>(sender);
        mSerialPorts.remove(serialPort->portName());
        delete serialPort;
    }
}