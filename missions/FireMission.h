#ifndef FIREMISSION_H
#define FIREMISSION_H
#include "Mission.h"

class FireMission : public Mission
{
public:
    FireMission();

    QString objective(QString string);
};

#endif // FIREMISSION_H
