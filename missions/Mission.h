#ifndef MISSION_H
#define MISSION_H
#include <QString>

class Mission
{
public:
    Mission() = default;
    virtual QString baseObjective(QString string) = 0;
    virtual QString bonusObjective(QString string) = 0;

    int base = 1;
    int bonus = 1;
};



#endif // MISSION_H
