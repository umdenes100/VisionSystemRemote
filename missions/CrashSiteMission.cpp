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
        return "The height of the side with abnormality is: " + string + " mm.\n";
        break;
    case 1:
        return "The width of the side with abnormality is : " + string + " mm.\n";
        break;
    case 2:
        return "The abnormality is in the " + string " direction.\n";
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
