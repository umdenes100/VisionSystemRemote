#ifndef MATERIALMISSION_H
#define MATERIALMISSION_H
#include "Mission.h"

class MaterialMission : public Mission
{
public:
    MaterialMission();

    QString objective(QString string);
};

#endif // MATERIALMISSION_H
