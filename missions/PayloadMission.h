#ifndef PAYLOADMISSION_H
#define PAYLOADMISSION_H

#include "Mission.h"

class PayloadMission : public Mission
{
public:
    PayloadMission();

    QString bonusObjective(QString string);
};

#endif // PAYLOADMISSION_H
