#ifndef DEBRISMISSION_H
#define DEBRISMISSION_H
#include "Mission.h"

class DebrisMission : public Mission
{
public:
    DebrisMission();

    QString objective(QString string);
};

#endif // DEBRISMISSION_H
