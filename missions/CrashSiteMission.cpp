#include "CrashSiteMission.h"

#include <QString>

CrashSiteMission::CrashSiteMission() : Mission()
{
}

QString CrashSiteMission::objective(QString string) {
    /*
     * The first call is height.
     * The second call is width.
     * Later calls are ignored.
     */
    switch(callNumber++) {
    case 0:
        return "The height of the payload is: " + string + "mm.\n";
        break;
    case 1:
        return "The width of the payload is : " + string + "mm.\n";
        break;
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
