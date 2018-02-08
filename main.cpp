#include <QCoreApplication>

#include "Controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Controller c();
    //c.start();

    return a.exec();
}
