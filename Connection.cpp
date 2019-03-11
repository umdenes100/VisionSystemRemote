#include "Connection.h"

#include <QDebug>
#include <QDateTime>
#include "missions/BlackBoxMission.h"
#include "missions/ChemicalMission.h"
#include "missions/DebrisMission.h"
#include "missions/FireMission.h"
#include "missions/WaterMission.h"

Connection::Connection(QString info, Arena& arena) :
    mArena(arena),
    mTeamName(info)
{
    mType = "BLACK_BOX";
    mAddress = info;
    mission = new BlackBoxMission();
    mTimeCheck = new QTimer();
    connect(mTimeCheck, SIGNAL(timeout()), this, SLOT(checkTime()));
}

Connection::~Connection()
{

}

QString& Connection::getTeamName(){
    return mTeamName;
}

QString Connection::getTeamType(){
    return mType;
}

void Connection::checkTime() {
    emit newCommand(mAddress, "TIME", QString::number(QDateTime::currentSecsSinceEpoch()));
}

void Connection::processDatagram(QNetworkDatagram data)
{
    processCommand(QString(data.data()));
}

void Connection::processCommand(QString buffer)
{
    switch ((int)mBuffer[0].unicode()) {
    case 0:
        if (buffer.length() == 1) {
            emit write(mAddress, QByteArray().append("\x01"));
        }
        break;
    case 2:
        if (buffer.length() > 2) {

            QByteArray response = QByteArray();
            if (mType != "BLACK_BOX") {
                Position destination = mArena.getTargetLocation();
                response.append(destination.serialize());
            } else {
                response = Position().serialize();
            }

            emit write(mAddress, QByteArray().append("\x03").append(response));

            running = true;
            mTeamName = buffer.mid(2);
            delete mission;

            switch((int)buffer[1].unicode()) {
            case 0:
                mType = "BLACK_BOX";
                mission = new BlackBoxMission();
                break;
            case 1:
                mType = "CHEMICAL";
                mission = new ChemicalMission();
                break;
            case 2:
                mType = "DEBRIS";
                mission = new DebrisMission();
                break;
            case 3:
                mType = "FIRE";
                mission = new FireMission();
                break;
            case 4:
                mType = "WATER";
                mission = new WaterMission();
                break;
            default:
                mType = "BLACK_BOX";
                mission = new BlackBoxMission();
                break;
            }

            mTimeCheck->start(500);
            emit newName();

            emit newCommand(mAddress, "START", QString::number(QDateTime::currentSecsSinceEpoch()));
        }
        break;
    case 4:
        if (buffer.length() == 3) {
            int markerId = buffer[1].unicode() + (buffer[2].unicode() << 8);

            Marker marker(markerId);
            bool markerPresent = mArena.getPosition(markerId, marker);
            if (markerPresent) {
                QByteArray response = QByteArray("\x05").append(marker.serialize());
                emit write(mAddress, response);
            } else {
                emit write(mAddress, QByteArray("\x09"));
            }
        }
        break;
    case 6:
        {
            QString param = buffer.mid(1);
            QString result = mission->objective(param);
            emit write(mAddress, QByteArray().append("\x07"));
            emit newCommand(mAddress, "MISSION", result);
        }

        break;
    case 8:
        emit newMessage(mAddress, buffer.mid(1));
        break;
    default:
        break;
    }

    mBuffer.clear();
}
