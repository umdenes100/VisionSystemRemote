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
    QString buffer;

    while (bytesAvailable()) {
        char c;
        getChar(&c);

        if (c == '#') {
            commandMode = true;
            emit newMessage(portName(), buffer);
            buffer = QString("");
        } else {
           if (commandMode) {
                if(c == '*') {
                    processCommand(buffer);
                    commandMode = false;
                }else {
                    buffer.append(c);
                }
           } else {
                buffer.append(c);
                if(c == '\n') {
                    emit newMessage(portName(), buffer);
                    buffer = QString("");
                }
           }
        }
    }


}

void SerialPort::processCommand(QString buffer){
    // if we get startMission then the list of serialPorts has changed:
    if (buffer.contains("start")) {
        // #start <teamType> <teamName>*
        QStringList values = buffer.split(' ');

        QString teamName = "";
        for (int i = 2; i < values.size(); i++) {
            teamName += values[i] + " ";
        }

        mTeamName = teamName;
        emit newName();
    }
}
