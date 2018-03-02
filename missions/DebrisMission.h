#ifndef DEBRISMISSION_H
#define DEBRISMISSION_H
#include "Mission.h"

class DebrisMission : public Mission
{
public:
    DebrisMission();

    QString baseObjective(QString string);
    QString bonusObjective(QString string);
};

#endif // DEBRISMISSION_H
