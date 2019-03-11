#ifndef CONNECTIONLIST_H
#define CONNECTIONLIST_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QMutex>
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
    QMutex mConnectionListMutex;

private slots:
    void readPendingDatagrams();
    void onWrite(QString to, QByteArray data);

public slots:
    void onNewMessage(QString connectionName, QString message);
    void onNewName();
    void onNewCommand(QString connectionName, QString type, QString message);

signals:
    void newMessage(QString connectionName, QString message);
    void newName();
    void newCommand(QString connectionName, QString type, QString message);
    void newConnection(QString connectionName);

private:
    Arena& mArena;
    QUdpSocket *mUdpSocket;
    QMap<QString, Connection*> mConnections;
};

#endif // CONNECTIONLIST_H
