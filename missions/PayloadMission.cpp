#include "PayloadMission.h"

#include <QString>

PayloadMission::PayloadMission() : Mission()
{
}

QString PayloadMission::bonusObjective(QString string){
    if (bonus > 0){
        bonus--;
        return "The Message is: " + string + "\n";
    }else {
        return "ERROR: too many base objective calls\n";
    }
}
