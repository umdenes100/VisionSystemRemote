#include "DataMission.h"

#include <QString>

DataMission::DataMission() : Mission()
{
}

QString DataMission::objective(QString string) {
    // first call is location
    // later calls are ignored
    switch(callNumber++) {
    case 0:
        return "The Message is: " + string + "\n";
        break;
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
