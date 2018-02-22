#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include <QImage>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

public slots:
    void onNewFrame(QImage frame);
    void onNewMessage(QString buffer);

signals:

private slots:
    void onNewImageConnection();

private:
    QTcpServer mImageServer;

    QList<QTcpSocket*> mImageClients;
};

#endif // SERVER_H
