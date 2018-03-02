#ifndef WATERMISSION_H
#define WATERMISSION_H
#include "Mission.h"

class WaterMission : public Mission
{
public:
    WaterMission();

    QString baseObjective(QString string);
    QString bonusObjective(QString string);
};

#endif // WATERMISSION_H
