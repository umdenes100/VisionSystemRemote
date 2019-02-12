#ifndef MISSION_H
#define MISSION_H
#include <QString>

class Mission
{
public:
    Mission() = default;
    virtual QString objective(QString string) = 0;

    int callNumber = 0;
};



#endif // MISSION_H
