#include "DebrisMission.h"

#include <QString>
#include <QDebug>

DebrisMission::DebrisMission() : Mission()
{
}

QString DebrisMission::objective(QString string) {
    // first call is location
    // later calls are ignored
    switch(callNumber++) {
    case 0:
        {
            int state = string.toInt();
            QString materialType;

            if(state == 0){
                materialType = "Copper";
            }else if(state == 1){
                materialType = "Steel";
            }else{
                materialType = "ERROR";
            }

            return "The type of the material is: " + materialType + "\n";
        }
        break;
    case 1:
        return "The mass of the material is: " + string + " grams\n";
        break;
    default:
        return "ERROR: too many mission() calls\n";
        break;
    }
}
