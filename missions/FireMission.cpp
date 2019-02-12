#include "FireMission.h"

#include <QString>

FireMission::FireMission() : Mission()
{
}

QString FireMission::objective(QString string) {
    // first call is location
    // later calls are ignored
    switch(callNumber++) {
    case 0:
        return "The number of flames is: " + string + "\n";
        break;
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
