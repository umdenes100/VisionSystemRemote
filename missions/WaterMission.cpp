#include "WaterMission.h"

#include <QString>

WaterMission::WaterMission() : Mission()
{
}

QString WaterMission::objective(QString string) {
    /*
     * first call is water type
     * second call is pool depth
     * later calls are ignored
     */
    switch(callNumber++) {
    case 0:
            int state = string.toInt();
            QString waterType;

            switch(state){
                case 0:
                    waterType = "Fresh";
                    break;
                case 1:
                    waterType = "Polluted";
                    break;
                case 2:
                    waterType = "Salt";
                    break;
                case 3:
                    waterType = "Salt and Polluted";
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
