#include "Server.h"

#include <QTcpSocket>

Server::Server(QObject *parent) :
    QObject(parent),
    mImageServer()
{
    connect(&mImageServer, SIGNAL(newConnection()), SLOT(onNewImageConnection()));
    mImageServer.listen(QHostAddress::Any, 8080);
}

void Server::onNewImageConnection() {
    QTcpSocket* socket = mImageServer.nextPendingConnection();
    socket->write("HTTP/1.1 200 OK\r\n\r\n");
}

void Server::onNewMessage(QString buffer) {

}
