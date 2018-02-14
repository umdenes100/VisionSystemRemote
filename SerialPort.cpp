#include "SerialPort.h"

#include <QDebug>

SerialPort::SerialPort(QSerialPortInfo& info, Arena& arena) :
    QSerialPort(info),
    mArena(arena)
{
    connect(this, SIGNAL(readyRead()), SLOT(onReadyRead()));
    setBaudRate(QSerialPort::Baud9600);
    setDataBits(QSerialPort::Data8);
    setParity(QSerialPort::NoParity);
    setStopBits(QSerialPort::OneStop);
    setRequestToSend(false);
    open(QIODevice::ReadWrite);
}

void SerialPort::onReadyRead() {
    while (bytesAvailable()) {
        char c;
        getChar(&c);
        qDebug() << c;
    }
}