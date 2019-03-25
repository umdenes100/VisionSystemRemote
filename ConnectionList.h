#ifndef CONNECTIONLIST_H
#define CONNECTIONLIST_H

#include <QObject>
#include <QMap>
#include <QUdpSocket>

#include "Arena.h"
#include "Connection.h"

class ConnectionList : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionList(Arena& arena, QObject *parent = 0);
    QMap<QString, Connection*>& getMap();
    ~ConnectionList();

private slots:
    void readPendingDatagrams();

signals:
    void newMessage(QString connectionName, QString message);
    void newName();
    void newCommand(QString connectionName, QString type, QString message);
    void newConnection(QString connectionName);

private:
    QByteArray process(QString sender, QByteArray data);

    Arena& mArena;
    QUdpSocket *mUdpSocket;
    QMap<QString, Connection*> mConnections;
};

#endif // CONNECTIONLIST_H
