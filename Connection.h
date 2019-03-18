#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include "missions/Mission.h"

class Connection : public QObject
{
    Q_OBJECT

public:
    explicit Connection();
    ~Connection();
    QString& getTeamName();
    QString getTeamType();

private:
    void process(QByteArray buffer);
    
    QString mTeamName;
    QString mMissionType;
    Mission *mMission;
};

#endif // CONNECTION_H
