#include "WaterMission.h"

#include <QString>

WaterMission::WaterMission() : Mission()
{
}

QString WaterMission::objective(QString string) {
    // first call is location
    // later calls are ignored
    switch(callNumber++) {
    case 0:
        {
            int state = string.toInt();
            QString waterType;

            if(state == 0){
                waterType = "Fresh";
            }else if(state == 1){
                waterType = "Polluted";
            }else if(state == 2){
                waterType = "Salt";
            }else{
                waterType = "ERROR";
            }

            return "The state of the water is: " + waterType + "\n";
        }
        break;
    case 1:
        return "The depth of the pool is: " + string + " mm\n";
        break;
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
