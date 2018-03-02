#ifndef CHEMICALMISSION_H
#define CHEMICALMISSION_H

#include "Mission.h"

class ChemicalMission : public Mission
{
public:
    ChemicalMission();

    QString baseObjective(QString string);
    QString bonusObjective(QString string);
};

#endif // CHEMICALMISSION_H
