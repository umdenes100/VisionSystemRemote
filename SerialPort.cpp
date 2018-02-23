#include "SerialPort.h"

#include <QDebug>

SerialPort::SerialPort(QSerialPortInfo& info, Arena& arena) :
    QSerialPort(info),
    mArena(arena),
    mTeamName(info.portName())
{
    connect(this, SIGNAL(readyRead()), SLOT(onReadyRead()));
    setBaudRate(QSerialPort::Baud9600);
    setDataBits(QSerialPort::Data8);
    setParity(QSerialPort::NoParity);
    setStopBits(QSerialPort::OneStop);
    while(!isOpen())
        open(QIODevice::ReadWrite);
    setRequestToSend(false);
}

QString& SerialPort::getTeamName(){
    return mTeamName;
}

void SerialPort::onReadyRead() {
    bool commandMode = false;

    QString buffer;

    while (bytesAvailable()) {
        char c;
        getChar(&c);

        if (c == '#') {
            commandMode = true;
            emit transmit(mTeamName, buffer);
            buffer = QString("");
        } else {
           if (commandMode) {
                if(c == '*'){
                    commandMode = false;
                    processCommand(buffer);
                }else {
                    buffer.append(c);
                }
           } else {
                buffer.append(c);
                if(c == '\n') {
                    emit transmit(mTeamName, buffer);
                }
                buffer = QString("");
           }
        }
    }
}

void SerialPort::processCommand(QString buffer){

}