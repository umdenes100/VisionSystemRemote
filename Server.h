#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

public slots:
    void onNewMessage(QString buffer);

signals:

private slots:
    void onNewImageConnection();

private:
    QTcpServer mImageServer;
};

#endif // SERVER_H
