#include "ConnectionList.h"

#include <QNetworkDatagram>
#include <QDateTime>

ConnectionList::ConnectionList(Arena& arena, QObject *parent) :
    QObject(parent),
    mArena(arena)
{
    mUdpSocket = new QUdpSocket(this);
    connect(mUdpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    mUdpSocket->bind(QHostAddress::Any, 7755);
}

ConnectionList::~ConnectionList()
{
    delete mUdpSocket;
}

QMap<QString, Connection *>& ConnectionList::getMap() {
    return mConnections;
}

// Retrieves all pending datagrams and interprets their payloads
void ConnectionList::readPendingDatagrams() {
    while (mUdpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = mUdpSocket->receiveDatagram();
        QString sender = datagram.senderAddress().toString();

        if(!mConnections.contains(sender)) {
            Connection *connection = new Connection();
            mConnections.insert(sender, connection);
            emit newConnection(sender);
        }

        QByteArray response = process(sender, datagram.data().mid(1));
        if (!response.isEmpty()) {
            char seq = datagram.data()[0];
            mUdpSocket->writeDatagram(QByteArray().append(seq).append(response), datagram.senderAddress(), 7755);
        }
    }
}

// Interprets a datagram payload
QByteArray ConnectionList::process(QString sender, QByteArray data) {
    switch (data[0]) {
        case 0: {
            // Ping
            return QByteArray().append('\x01');
        }
        case 2: {
            // Start
            QString teamName = QString(data.mid(2));
            int missionType = data[1];
            mConnections[sender]->start(teamName, missionType);
            emit newName();
            emit newCommand(sender, "START", QString::number(QDateTime::currentSecsSinceEpoch()));

            if (missionType != 0) {
                Position destination = mArena.getTargetLocation();
                return QByteArray().append('\x03').append(destination.serialize());
            }
            return QByteArray().append('\x03').append(Position().serialize());
        }
        case 4: {
            // Location request
            int markerId = data[1] | data[2] << 8;
            Marker location(markerId);
            bool markerPresent = mArena.getPosition(markerId, location);
            if (markerPresent) {
                return QByteArray().append('\x05').append(location.serialize());
            }
            return QByteArray().append('\x09');
        }
        case 6: {
            // Mission message
            QString value = QString(data.mid(1));
            emit newCommand(sender, "MISSION", mConnections[sender]->mission(value));
            return QByteArray().append('\x07');
        }
        case 8: {
            // Debug message
            emit newMessage(sender, QString(data.mid(1)));
            return QByteArray();
        }
    }

    return QByteArray();
}
