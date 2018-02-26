#include "Server.h"
#include "SerialPort.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QBuffer>

Server::Server(Arena& arena, QObject *parent) :
    QObject(parent),
    mImageServer(),
    mMessageServer(QStringLiteral("LTFs"), QWebSocketServer::NonSecureMode, this),
    mSerialPortList(arena)
{
    connect(&mSerialPortList, SIGNAL(newMessage(QString,QString)), SLOT(onNewMessage(QString,QString)));
    connect(&mSerialPortList, SIGNAL(newSerialPort(QString)), SLOT(addNameToMap(QString)));
    connect(&mImageServer, SIGNAL(newConnection()), SLOT(onNewImageConnection()));
    connect(&mMessageServer, SIGNAL(newConnection()), SLOT(onNewMessageConnection()));

    qDebug() << "Starting image server...";
    mImageServer.listen(QHostAddress::Any, 8080);
    qDebug() << "Server listening on port 8080";

    qDebug() << "Starting message server...";
    mMessageServer.listen(QHostAddress::Any, 9000);
    qDebug() << "Server listening on port 9000";

}

void Server::addNameToMap(QString name){
    mMessageClients.insert(name, QList<QWebSocket *>());
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
void Server::onNewMessageConnection() {
    QWebSocket* socket = mMessageServer.nextPendingConnection();

    QMap<QString, SerialPort *> serialPorts = mSerialPortList.getMap();
    foreach(QString portName, serialPorts.keys()){
        //transmit portname, teamname
//        socket->sendTextMessage(portName);
    }
}

void Server::onNewMessage(QString portName, QString message) {
    foreach(QWebSocket* socket, mMessageClients[portName]){
        socket->sendTextMessage(message);
    }
}


