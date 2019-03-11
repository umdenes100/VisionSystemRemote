#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTimer>
#include <QNetworkDatagram>
#include "Arena.h"
#include "missions/Mission.h"

class Connection : public QObject
{
    Q_OBJECT

public:
    explicit Connection(QString info, Arena& arena);
    void processDatagram(QNetworkDatagram data);
    ~Connection();
    QString& getTeamName();
    QString getTeamType();
    QString mAddress;

signals:
    void newMessage(QString connectionName, QString message);
    void newName();
    void newCommand(QString connectionName, QString type, QString message);
    void write(QString self, QByteArray data);

private slots:
    void checkTime();

private:
    Arena& mArena;
    QString mTeamName;
    bool commandMode = false;
    bool running = false;
    QString mType;
    Mission *mission;
    QString mBuffer;
    QTimer *mTimeCheck;

    void processCommand(QString buffer);
};

#endif // CONNECTION_H
