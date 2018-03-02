#include "WaterMission.h"

#include <QString>

WaterMission::WaterMission() : Mission()
{
}

QString WaterMission::baseObjective(QString string){
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

    if (base > 0){
        base--;
        return "The state of the water is: " + waterType + "\n";
    }else {
        return "ERROR: too many base objective calls\n";
    }
}

QString WaterMission::bonusObjective(QString string){
    if (bonus > 0){
        bonus--;
        return "The depth of the pool is: " + string + " mm\n";
    }
    else {
        return "ERROR: too many bonus objective calls\n";
    }
}
