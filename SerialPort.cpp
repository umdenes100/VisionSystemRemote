#include "SerialPort.h"

#include <QDebug>
#include "missions/BlackBoxMission.h"
#include "missions/ChemicalMission.h"
#include "missions/DebrisMission.h"
#include "missions/FireMission.h"
#include "missions/WaterMission.h"

SerialPort::SerialPort(QSerialPortInfo& info, Arena& arena) :
    QSerialPort(info),
    mArena(arena),
    mTeamName(info.portName()),
    mType(BLACK_BOX)
{
    connect(this, SIGNAL(readyRead()), SLOT(onReadyRead()));
    setBaudRate(QSerialPort::Baud9600);
    setDataBits(QSerialPort::Data8);
    setParity(QSerialPort::NoParity);
    setStopBits(QSerialPort::OneStop);
    while(!isOpen())
        open(QIODevice::ReadWrite);
    setRequestToSend(false);

    mission = new BlackBoxMission();
}

QString& SerialPort::getTeamName(){
    return mTeamName;
}

TeamType SerialPort::getTeamType(){
    return mType;
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
    bool ok = true;
    int markerNumber = buffer.toInt(&ok);
    if (ok) {
        Marker marker(markerNumber);
        bool markerPresent = mArena.getPosition(markerNumber, marker);
        if(markerPresent){
            QString response = QString("") +
                    QString::number(marker.id) + QString(",") +
                    QString::number(marker.x) + QString(",") +
                    QString::number(marker.y) + QString(",") +
                    QString::number(marker.theta) + QString("\n");
            write(QByteArray().append(response));
        }
    }else {
        if (buffer.contains("start")) {
            running = true;
            // #start <teamType> <teamName>*
            QStringList list = buffer.split(" ").mid(0, 2);
            QString teamName = buffer.section(' ', 2);
            QString teamString = list[1];

            mType = static_cast<TeamType>(teamString.toInt());
            if (mType == BLACK_BOX) {
                mission = new BlackBoxMission();
            } else if (mType == CHEMICAL) {
                mission = new ChemicalMission();
            } else if (mType == DEBRIS) {
                mission = new DebrisMission();
            } else if (mType == FIRE) {
                mission = new FireMission();
            } else if (mType == WATER) {
                mission = new WaterMission();
            }

            mTeamName = teamName;
            emit newName();

            QString message;
            message += "\n*** MISSION MESSAGE ***\n";
            message += "Start of Mission\n";
            message += "**************************\n";
            emit newCommand(portName(), START, message);
        } else if (buffer.contains("base")) {
            QString param = buffer.split(" ")[1];
            QString result = mission->baseObjective(param);
            QString message;
            message += "\n*** MISSION MESSAGE ***\n";
            message += result;
            message += "**************************\n";
            emit newCommand(portName(), BASE, message);
        } else if (buffer.contains("bonus")) {
            QString param = buffer.split(" ")[1];
            QString result = mission->bonusObjective(param);
            QString message;
            message += "\n*** MISSION MESSAGE ***\n";
            message += result;
            message += "**************************\n";
            emit newCommand(portName(), BONUS, message);
        } else if (buffer.contains("end")) {
            if (running) {
                QString message;
                message += "\n*** MISSION MESSAGE ***\n";
                message += "End of mission\n";
                message += "**************************\n";
                running = false;
                emit newCommand(portName(), END, message);
            }
        }else if (buffer.contains("destination")) {
            QString response = QString("");
            if (mType != BLACK_BOX) {
                Position destination = mArena.getTargetLocation();
                response += QString::number(destination.x) + QString(",");
                response += QString::number(destination.y) + QString("\n");
            } else {
                response += QString("0,0\n");
            }
            write(QByteArray().append(response));
        } else if (buffer.contains("navigated")) {
            QString message;
            message += "\n*** MISSION MESSAGE ***\n";
            message += "Team has reached destination";
            message += "\n**************************\n";
            emit newCommand(portName(), NAVIGATED, message);
        }
    }
}

