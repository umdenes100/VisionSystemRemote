#include "Connection.h"

#include "missions/CrashSiteMission.h"
#include "missions/DataMission.h"
#include "missions/MaterialMission.h"
#include "missions/FireMission.h"
#include "missions/WaterMission.h"

Connection::Connection() :
    mMissionType("CRASH_SITE"),
    mMission(new CrashSiteMission())
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
            mMissionType = "CRASH_SITE";
            mMission = new CrashSiteMission();
            break;
        case 1:
            mMissionType = "DATA";
            mMission = new DataMission();
            break;
        case 2:
            mMissionType = "MATERIAL";
            mMission = new MaterialMission();
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
            mMissionType = "CRASH_SITE";
            mMission = new CrashSiteMission();
            break;
    }
}

QString Connection::mission(QString value) {
    return mMission->objective(value);
}
