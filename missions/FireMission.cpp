#include "FireMission.h"

#include <QString>

FireMission::FireMission() : Mission()
{
}

QString FireMission::baseObjective(QString string){
    if (base > 0){
        base--;
        return "The type of the material is: " + string + "\n";
    }else {
        return "ERROR: too many base objective calls\n";
    }
}

QString FireMission::bonusObjective(QString string){
    return "ERROR: too many bonus objective calls\n";
}
