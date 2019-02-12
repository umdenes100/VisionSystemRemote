#ifndef BLACKBOXMISSION_H
#define BLACKBOXMISSION_H
#include "Mission.h"
#include <QString>

class BlackBoxMission : public Mission
{
public:
    BlackBoxMission();

    QString objective(QString string);
};

#endif // BLACKBOXMISSION_H
