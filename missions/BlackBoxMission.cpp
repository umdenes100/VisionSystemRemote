#include "BlackBoxMission.h"

#include <QString>

BlackBoxMission::BlackBoxMission() : Mission()
{   
}

QString BlackBoxMission::objective(QString string) {
    // first call is location
    // later calls are ignored
    switch(callNumber++) {
    case 0:
        return "The Color of the LED is: (" + string + ")\n";
        break;
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
