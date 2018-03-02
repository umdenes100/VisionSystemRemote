#include "Server.h"
#include "SerialPort.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QBuffer>
#include <QPair>
#include <QJsonObject>
#include <QJsonDocument>

Server::Server(Arena& arena, QObject *parent) :
    QObject(parent),
    mImageServer(),
    mMessageServer(QStringLiteral("LTFs"), QWebSocketServer::NonSecureMode, this),
    mSerialPortList(arena)
{
    connect(&mSerialPortList, SIGNAL(newMessage(QString,QString)), SLOT(onNewMessage(QString,QString)));
    connect(&mSerialPortList, SIGNAL(newSerialPort(QString)), SLOT(addNameToMap(QString)));
    connect(&mSerialPortList, SIGNAL(newName()), SLOT(onNewName()));
    connect(&mSerialPortList, SIGNAL(newCommand(QString,CommandType,QString)), SLOT(onNewCommand(QString,CommandType,QString)));
    connect(&mImageServer, SIGNAL(newConnection()), SLOT(onNewImageConnection()));
    connect(&mMessageServer, SIGNAL(newConnection()), SLOT(onNewMessageConnection()));

    qDebug() << "Starting image server...";
    mImageServer.listen(QHostAddress::Any, 8080);
    qDebug() << "Server listening on port 8080";

    qDebug() << "Starting message server...";
    mMessageServer.listen(QHostAddress::Any, 9000);
    qDebug() << "Server listening on port 9000";

    mMessageClients.insert("", QList<QWebSocket *>());
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
        QByteArray frameString = ("--newframe\r\n" \
                      "Content-Type: image/jpeg\r\n" \
                      "Content-Length: ");

        frameString.append(QString::number(bin.length()));
        frameString.append("\r\n\r\n");

        socket->write(frameString);
        socket->write(bin);
    }
}

void Server::onNewMessageConnection() {
    QWebSocket* socket = mMessageServer.nextPendingConnection();

    connect(socket, SIGNAL(textMessageReceived(QString)), SLOT(onMessageReceived(QString)));

    QMap<QString, SerialPort *> serialPorts = mSerialPortList.getMap();
    QString json = jsonify(serialPorts);
    socket->sendTextMessage(json);

    mMessageClients[""].append(socket);
}

void Server::onNewMessage(QString portName, QString message) {
    QString jsonMessage = jsonify(message);
    foreach(QWebSocket* socket, mMessageClients[portName]){
        socket->sendTextMessage(jsonMessage);
    }
}

void Server::onNewName() {
    QMap<QString, SerialPort *> serialPorts = mSerialPortList.getMap();
    QString json = jsonify(serialPorts);

    foreach (QList<QWebSocket*> portClients, mMessageClients.values()) {
        foreach (QWebSocket* socket, portClients) {
            socket->sendTextMessage(json);
        }
    }
}

void Server::onMessageReceived(QString message) {
    qDebug() << message;
    if(mMessageClients.contains(message)){
        qDebug() << "found";

        QWebSocket* socket = static_cast<QWebSocket *>(QObject::sender());
        foreach (QString key, mMessageClients.keys()) {
            mMessageClients[key].removeOne(socket);
        }

        mMessageClients[message].append(socket);
    }
}

void Server::onNewCommand(QString portName, CommandType type, QString message) {
    QString jsonMessage = jsonify(type, message);
    foreach(QWebSocket* socket, mMessageClients[portName]){
        socket->sendTextMessage(jsonMessage);
    }
}

QString Server::jsonify(QMap<QString, SerialPort *> serialPorts) {
   QJsonObject jsonPorts, jsonMessage;

   jsonMessage.insert("TYPE", "PORTLIST");

   foreach(QString portName, serialPorts.keys()){
       QJsonObject json;
       json.insert("NAME", serialPorts[portName]->getTeamName());
       json.insert("MISSION", serialPorts[portName]->getTeamType());
       jsonPorts.insert(portName, json);
   }

   jsonMessage.insert("CONTENT", jsonPorts);
   QJsonDocument doc(jsonMessage);
   return QString(doc.toJson(QJsonDocument::Compact));
}

QString Server::jsonify(QString message) {
    QJsonObject jsonMessage, jsonContents;

    jsonMessage.insert("TYPE", "MESSAGE");
    jsonContents.insert("M_TYPE", "DEBUG");
    jsonContents.insert("CONTENT", message);
    jsonMessage.insert("CONTENT", jsonContents);

    QJsonDocument doc(jsonMessage);
    return QString(doc.toJson(QJsonDocument::Compact));
}

QString Server::jsonify(CommandType type, QString message){
    QJsonObject jsonMessage, jsonContents;

    jsonMessage.insert("TYPE", "MESSAGE");
    jsonContents.insert("M_TYPE", "MISSION");
    jsonContents.insert("CONTENT_TYPE", type);
    jsonContents.insert("CONTENT", message);
    jsonMessage.insert("CONTENT", jsonContents);

    QJsonDocument doc(jsonMessage);
    return QString(doc.toJson(QJsonDocument::Compact));
}




