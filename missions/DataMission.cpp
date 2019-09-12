#include "DataMission.h"

#include <QString>

DataMission::DataMission() : Mission()
{
}

QString DataMission::bonusObjective(QString string){
    if (bonus > 0){
        bonus--;
        return "The Message is: " + string + "\n";
    }else {
        return "ERROR: too many base objective calls\n";
    }
}
