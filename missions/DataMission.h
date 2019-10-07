#ifndef DATAMISSION_H
#define DATAMISSION_H

#include "Mission.h"

class DataMission : public Mission
{
public:
    DataMission();

    QString objective(QString string);
};

#endif // DATAMISSION_H
