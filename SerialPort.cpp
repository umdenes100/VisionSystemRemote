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
    while (bytesAvailable()) {
        char c;
        getChar(&c);
        if (c == (char)255) {
            processCommand(mBuffer);
        } else {
            mBuffer.append(c);
        }
    }
}

void SerialPort::processCommand(QString buffer){
    switch ((int)mBuffer[0].unicode()) {
    case 0:
        if (buffer.length() == 1) {
            write(QByteArray().append("\x01"));
        }
        break;
    case 2:
        qDebug() << "start mission";
        if (buffer.length() > 2) {

            QByteArray response = QByteArray();
            if (mType != BLACK_BOX) {
                Position destination = mArena.getTargetLocation();
                response.append(destination.serialize());
            } else {
                response = Position().serialize();
            }

            write(QByteArray().append("\x03").append(response));

            running = true;
            mTeamName = buffer.mid(2);
            mType = static_cast<TeamType>((int)buffer[1].unicode());

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

            emit newName();

            QString message;
            message += "\n*** MISSION MESSAGE ***\n";
            message += "Start of Mission\n";
            message += "**************************\n";
            emit newCommand(portName(), START, message);
        }
        break;
    case 4:
        if (buffer.length() == 3) {
            int markerId = buffer[1].unicode() + (buffer[2].unicode() << 8);

            Marker marker(markerId);
            bool markerPresent = mArena.getPosition(markerId, marker);
            if (markerPresent) {
                QByteArray response = QByteArray("\x05").append(marker.serialize());
                write(response);
            }
        }
        break;
    case 6:
        if ((int)buffer[1].unicode()) {
            // if we have bonus
            QString param = buffer.mid(2);
            QString result = mission->bonusObjective(param);
            QString message;
            message += "\n*** MISSION MESSAGE ***\n";
            message += result;
            message += "**************************\n";
            write(QByteArray().append("\x07"));
            emit newCommand(portName(), BONUS, message);
        } else {
            QString param = buffer.mid(2);
            QString result = mission->baseObjective(param);
            QString message;
            message += "\n*** MISSION MESSAGE ***\n";
            message += result;
            message += "**************************\n";
            write(QByteArray().append("\x07"));
            emit newCommand(portName(), BASE, message);
        }

        break;
    case 8:
        emit newMessage(portName(), buffer.mid(1));
        break;
    default:
        break;
    }

    mBuffer.clear();

}

