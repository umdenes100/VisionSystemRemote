#include "BlackBoxMission.h"

#include <QString>

BlackBoxMission::BlackBoxMission() : Mission()
{   
}

QString BlackBoxMission::objective(QString string) {
    /* The first call is LED color.
     * Later calls are ignored.
     */
    switch(callNumber++) {
    case 0:
        return "The Color of the LED is: (" + string + ")\n";
        break;
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
