#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>
#include <QImage>
#include <QMap>
#include <QTimer>

#include "ConnectionList.h"


class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(ConnectionList* list, QObject *parent = 0);

public slots:
    void onNewFrame(QImage frame);

private slots:
    void start();
    void onNewImageConnection();
    void onNewMessageConnection();
    void addNameToMap(QString name);
    void onNewMessage(QString portName, QString message);
    void onNewName();
    void onMessageReceived(QString message);
    void onNewCommand(QString portName, QString type, QString message);
    void onImageConnectionEnded();
    void onMessageConnectionEnded();
    void onTimeCheck();

private:
    QString jsonify(QMap<QString, Connection*>);
    QString jsonify(QString);
    QString jsonify(QString type, QString message);

    QTcpServer mImageServer;
    QList<QTcpSocket*> mImageClients;

    QWebSocketServer mMessageServer;
    QMap<QString, QList<QWebSocket*>> mMessageClients;

    ConnectionList* mConnectionList;

    QTimer mTimeCheckTimer;
};

#endif // SERVER_H
