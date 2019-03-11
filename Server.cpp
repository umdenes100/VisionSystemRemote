#include "Server.h"
#include "Connection.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QBuffer>
#include <QPair>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMutex>

Server::Server(ConnectionList* list, QObject *parent) :
    QObject(parent),
    mImageServer(),
    mMessageServer(QStringLiteral("LTFs"), QWebSocketServer::NonSecureMode, this),
    mConnectionList(list)
{
    connect(&mImageServer, SIGNAL(newConnection()), SLOT(onNewImageConnection()));
    connect(&mMessageServer, SIGNAL(newConnection()), SLOT(onNewMessageConnection()));
}

void Server::start() {
    qDebug() << "Starting image server...";
    mImageServer.listen(QHostAddress::Any, 8080);
    qDebug() << "Server listening on port 8080";

    qDebug() << "Starting message server...";
    mMessageServer.listen(QHostAddress::Any, 9000);
    qDebug() << "Server listening on port 9000";

    mMessageClients.insert("", QList<QWebSocket *>());
}

void Server::addNameToMap(QString name) {
    mMessageClients.insert(name, QList<QWebSocket *>());
}

void Server::onNewImageConnection() {
    QTcpSocket* socket = mImageServer.nextPendingConnection();

    connect(socket, SIGNAL(disconnected()), SLOT(onImageConnectionEnded()));
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
    connect(socket, SIGNAL(disconnected()), SLOT(onMessageConnectionEnded()));
    mConnectionList->mConnectionListMutex.lock();
    QMap<QString, Connection *> serialPorts = mConnectionList->getMap();
    QString json = jsonify(serialPorts);
    mConnectionList->mConnectionListMutex.unlock();
    socket->sendTextMessage(json);

    mMessageClients[""].append(socket);
}

void Server::onNewMessage(QString portName, QString message) {
    QString jsonMessage = jsonify("DEBUG", message);
    foreach(QWebSocket* socket, mMessageClients[portName]){
        socket->sendTextMessage(jsonMessage);
    }
}

void Server::onNewName() {
    mConnectionList->mConnectionListMutex.lock();
    QMap<QString, Connection *> serialPorts = mConnectionList->getMap();
    QString json = jsonify(serialPorts);
    mConnectionList->mConnectionListMutex.unlock();

    foreach (QList<QWebSocket*> portClients, mMessageClients.values()) {
        foreach (QWebSocket* socket, portClients) {
            socket->sendTextMessage(json);
        }
    }
}

void Server::onMessageReceived(QString message) {
    QJsonObject jsonObject = QJsonDocument::fromJson(message.toUtf8()).object();

    QString type = jsonObject.value("TYPE").toString();
    QString port = jsonObject.value("PORT").toString();

    qDebug() << message;

    QWebSocket* socket = static_cast<QWebSocket *>(QObject::sender());
    if (type == "OPEN")  {
        mMessageClients[port].append(socket);
    } else if (type == "SWITCH") {
        QString newPort = jsonObject.value("NEW_PORT").toString();
        mMessageClients[port].removeOne(socket);
        mMessageClients[newPort].append(socket);
    } else if (type == "SOFT_CLOSE") {
        mMessageClients[port].removeOne(socket);
    } else if (type == "HARD_CLOSE") {
        mMessageClients[port].removeOne(socket);
        delete socket;
    }
}

void Server::onNewCommand(QString portName, QString type, QString message) {
    QString jsonMessage = jsonify(type, message);
    // qDebug() << jsonMessage;
    foreach(QWebSocket* socket, mMessageClients[portName]){
        socket->sendTextMessage(jsonMessage);
    }
}

void Server::onImageConnectionEnded() {
    QTcpSocket* socket = static_cast<QTcpSocket*>(QObject::sender());
    mImageClients.removeOne(socket);
}

void Server::onMessageConnectionEnded() {
    QWebSocket* socket = static_cast<QWebSocket*>(QObject::sender());
    foreach(QString key, mMessageClients.keys()){
        mMessageClients[key].removeOne(socket);
    }
}


QString Server::jsonify(QString type, QString message){
    QJsonObject jsonMessage;

    jsonMessage.insert("TYPE", type);
    jsonMessage.insert("CONTENT", message);

    QJsonDocument doc(jsonMessage);
    return QString(doc.toJson(QJsonDocument::Compact));
}

QString Server::jsonify(QMap<QString, Connection *> connections) {
   QJsonObject jsonPorts, jsonMessage;

   jsonMessage.insert("TYPE", "PORT_LIST");

   foreach(QString name, connections.keys()){
       QJsonObject json;
       json.insert("NAME", connections[name]->getTeamName());
       json.insert("MISSION", connections[name]->getTeamType());
       jsonPorts.insert(name, json);
   }

   jsonMessage.insert("CONTENT", jsonPorts);
   QJsonDocument doc(jsonMessage);
   return QString(doc.toJson(QJsonDocument::Compact));
}
