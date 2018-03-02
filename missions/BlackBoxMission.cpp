#include "BlackBoxMission.h"

#include <QString>

BlackBoxMission::BlackBoxMission() : Mission()
{   
}

QString BlackBoxMission::baseObjective(QString string){
    if (base > 0) {
        base--;
        return "The Black Box is at: (" + string + ")\n";
    } else {
        return "ERROR: too many base objective calls\n";
    }
}

QString BlackBoxMission::bonusObjective(QString string){
    return "ERROR: too many bonus objective calls\n";
}
