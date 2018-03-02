#include <QApplication>

#include "Controller.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Controller c;
    c.start();

    return a.exec();
}
