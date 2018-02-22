#include "Server.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QBuffer>

Server::Server(QObject *parent) :
    QObject(parent),
    mImageServer()
{
    connect(&mImageServer, SIGNAL(newConnection()), SLOT(onNewImageConnection()));

    qDebug() << "Starting image server...";
    mImageServer.listen(QHostAddress::Any, 8080);
    qDebug() << "Server listening on port 8080";
}

void Server::onNewImageConnection() {
    QTcpSocket* socket = mImageServer.nextPendingConnection();
    socket->write("HTTP/1.1 200 OK\r\n");
    socket->write("Content-Type: multipart/x-mixed-replace; boundary=newframe\r\n");
    socket->write("\r\n");

    mImageClients.append(socket);
}

void Server::onNewFrame(QImage frame) {
    QByteArray bin;
    QBuffer buffer(&bin);
    buffer.open(QIODevice::WriteOnly);
    frame.save(&buffer, "jpeg");

    foreach (QTcpSocket* socket, mImageClients) {
        socket->write("\r\n--newframe\r\n");
        socket->write("Content-Type: image/jpeg\r\n");
        socket->write("\r\n");
        socket->write(bin);
    }
}

void Server::onNewMessage(QString buffer) {

}
