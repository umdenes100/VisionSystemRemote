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

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

    void addNameToMap(QString& name);

public slots:
    void onNewFrame(QImage frame);
    void onNewMessage(QString teamName, QString buffer);

signals:

private slots:
    void onNewImageConnection();
    void onNewMessageConnection();

private:
    QTcpServer mImageServer;
    QList<QTcpSocket*> mImageClients;

    QWebSocketServer mMessageServer;
    QMap<QString, QList<QWebSocket*>> mMessageClients;

};

#endif // SERVER_H
