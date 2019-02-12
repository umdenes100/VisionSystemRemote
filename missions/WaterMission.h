#ifndef WATERMISSION_H
#define WATERMISSION_H
#include "Mission.h"

class WaterMission : public Mission
{
public:
    WaterMission();

    QString objective(QString string);
};

#endif // WATERMISSION_H
