#include <QApplication>

#include "Controller.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller c;
    MainWindow w(c);

    w.show();
    c.start();

    return a.exec();
}
