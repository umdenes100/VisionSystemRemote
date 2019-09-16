#include "WaterMission.h"

#include <QString>

WaterMission::WaterMission() : Mission()
{
}

QString WaterMission::objective(QString string) {
    /*
     * The first call is water type.
     * The second call is pool depth.
     * Later calls are ignored.
     */
    switch(callNumber++) {
    case 0:
            int state = string.toInt();
            QString waterType;

            switch(state){
                case 0:
                    waterType = "Fresh and Unpolluted";
                    break;
                case 1:
                    waterType = "Fresh and Polluted";
                    break;
                case 2:
                    waterType = "Salty and Unpolluted";
                    break;
                case 3:
                    waterType = "Salty and Polluted";
                    break;
                default:
                    waterType = "Error"
                    break;
            }
            return "The state of the water is: " + waterType + "\n";
            break;
    case 1:
        return "The depth of the pool is: " + string + " mm\n";
        break;
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
