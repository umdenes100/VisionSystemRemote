#include "ConnectionList.h"
#include <QWebSocket>
#include <QMutexLocker>
#include <QDebug>
#include <QNetworkDatagram>

ConnectionList::ConnectionList(Arena& arena, QObject *parent) :
    QObject(parent),
    mArena(arena)
{
    mUdpSocket = new QUdpSocket(this);
    mUdpSocket->bind(QHostAddress::LocalHost, 7755);

    connect(mUdpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

ConnectionList::~ConnectionList()
{
    delete mUdpSocket;
}

QMap<QString, Connection *>& ConnectionList::getMap(){
    return mConnections;
}

void ConnectionList::onWrite(QString to, QByteArray data)
{
    mUdpSocket->writeDatagram(QNetworkDatagram(data, QHostAddress(to.mid(0, to.indexOf(":"))), to.mid(to.indexOf(":") + 1).toUShort()));
}

void ConnectionList::readPendingDatagrams()
{
    while (mUdpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = mUdpSocket->receiveDatagram();

        QString sender = datagram.senderAddress().toString() + ":" + QString::number(datagram.senderPort());

        mConnectionListMutex.lock();
        if(!mConnections.contains(sender)) {
            Connection *connection = new Connection(sender, mArena);
            // connect(connection, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(onError(QSerialPort::SerialPortError)));
            connect(connection, SIGNAL(newMessage(QString, QString)), this, SLOT(onNewMessage(QString, QString)));
            connect(connection, SIGNAL(write(QString,QByteArray)), this, SLOT(onWrite(QString,QByteArray)));
            connect(connection, SIGNAL(newName()), this, SLOT(onNewName()));
            connect(connection, SIGNAL(newCommand(QString,QString,QString)), SLOT(onNewCommand(QString,QString,QString)));
            mConnections.insert(connection->mAddress, connection);
            emit newConnection(connection->mAddress);
        }
        mConnectionListMutex.unlock();

        mConnections[sender]->processDatagram(datagram);
    }
}

void ConnectionList::onNewMessage(QString connectionName, QString message){
    emit newMessage(connectionName, message);
}

void ConnectionList::onNewName() {
    emit newName();
}

void ConnectionList::onNewCommand(QString connectionName, QString type, QString message) {
    // qDebug() << portName << ": " << message;
    emit newCommand(connectionName, type, message);
}
