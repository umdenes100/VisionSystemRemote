#include "DebrisMission.h"

#include <QString>
#include <QDebug>

DebrisMission::DebrisMission() : Mission()
{
}

QString DebrisMission::baseObjective(QString string){
    int state = string.toInt();
    QString materialType;

    if(state == 0){
        materialType = "Copper";
    }else if(state == 1){
        materialType = "Steel";
    }else{
        materialType = "ERROR";
    }

    if (base > 0){
        base--;
        return "The type of the material is: " + materialType + "\n";
    }else {
        return "ERROR: too many base objective calls\n";
    }
}

QString DebrisMission::bonusObjective(QString string){
    if (bonus > 0){
        bonus--;
        return "The mass of the material is: " + string + " grams\n";
    }
    else {
        return "ERROR: too many bonus objective calls";
    }
}
