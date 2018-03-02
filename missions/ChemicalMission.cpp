#include "ChemicalMission.h"

#include <QString>

ChemicalMission::ChemicalMission() : Mission()
{
}

QString ChemicalMission::baseObjective(QString string){
    if (base > 0){
        base--;
        return "The pH of the pool is: " + string + "\n";
    }else {
        return "ERROR: too many base objective calls\n";
    }
}

QString ChemicalMission::bonusObjective(QString string){
    if (bonus > 0){
        bonus--;
        return "The final pH of the pool is: " + string + "\n";
    }
    else {
        return "ERROR: too many bonus objective calls\n";
    }
}
