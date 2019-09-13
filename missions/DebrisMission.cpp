#include "DebrisMission.h"

#include <QString>
#include <QDebug>

DebrisMission::DebrisMission() : Mission()
{
}

QString DebrisMission::objective(QString string) {
    /* 
     * The first call is material type.
     * The second call is weight.
     * Later calls are ignored.
     */
    switch(callNumber++) {
    case 0:
        {
            int state = string.toInt();
            QString materialType;

            switch(state) {
                case 0:
                    materialType = "Plastic";
                    break;
                case 1:
                    materialType = "Aluminum";
                    break;
                default:
                    materialType = "ERROR";
                    break;
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
