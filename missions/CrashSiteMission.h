#ifndef CRASHSITEMISSION_H
#define CRASHSITEMISSION_H
#include "Mission.h"
#include <QString>

class CrashSiteMission : public Mission
{
public:
    CrashSiteMission();

    QString objective(QString string);
};

#endif // CRASHSITEMISSION_H