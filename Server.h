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

#include "SerialPortList.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(Arena& arena, QObject *parent = 0);

public slots:
    void onNewFrame(QImage frame);

signals:

private slots:
    void onNewImageConnection();
    void onNewMessageConnection();
    void addNameToMap(QString name);
    void onNewMessage(QString portName, QString message);

private:
    QTcpServer mImageServer;
    QList<QTcpSocket*> mImageClients;

    QWebSocketServer mMessageServer;
    QMap<QString, QList<QWebSocket*>> mMessageClients;

    SerialPortList mSerialPortList;

};

#endif // SERVER_H
