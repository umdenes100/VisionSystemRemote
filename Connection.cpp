#include "Connection.h"

#include "missions/BlackBoxMission.h"
#include "missions/DataMission.h"
#include "missions/DebrisMission.h"
#include "missions/FireMission.h"
#include "missions/WaterMission.h"

Connection::Connection() :
    mMissionType("BLACK_BOX"),
    mMission(new BlackBoxMission())
{
}

Connection::~Connection()
{
    delete mMission;
}

QString& Connection::getTeamName() {
    return mTeamName;
}

QString Connection::getTeamType() {
    return mMissionType;
}

void Connection::start(QString teamName, int missionType) {
    mTeamName = teamName;
    mMissionType = missionType;

    delete mMission;
    switch (missionType) {
        case 0:
            mMissionType = "BLACK_BOX";
            mMission = new BlackBoxMission();
            break;
        case 1:
            mMissionType = "DATA_MISSION";
            mMission = new DataMission();
            break;
        case 2:
            mMissionType = "DEBRIS";
            mMission = new DebrisMission();
            break;
        case 3:
            mMissionType = "FIRE";
            mMission = new FireMission();
            break;
        case 4:
            mMissionType = "WATER";
            mMission = new WaterMission();
            break;
        default:
            mMissionType = "BLACK_BOX";
            mMission = new BlackBoxMission();
            break;
    }
}

QString Connection::mission(QString value) {
    return mMission->objective(value);
}
