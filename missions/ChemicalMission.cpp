#include "ChemicalMission.h"

#include <QString>

ChemicalMission::ChemicalMission() : Mission()
{
}

QString ChemicalMission::objective(QString string) {
    // first call is location
    // later calls are ignored
    switch(callNumber++) {
    case 0:
        return "The pH of the pool is: " + string + "\n";
        break;
    case 1:
        return "The final pH of the pool is: " + string + "\n";
        break;
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
