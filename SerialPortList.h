#ifndef SERIALPORTLIST_H
#define SERIALPORTLIST_H

#include <QObject>
#include <QTimer>

#include "Arena.h"

class SerialPortList : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortList(Arena& arena, QObject *parent = 0);

private slots:
    void refreshPorts();
    void onPortAboutToClose();

private:
    Arena& mArena;
    QTimer mRefreshPortsTimer;
    QMap<QString, SerialPort*> mSerialPorts;
};

#endif // SERIALPORTLIST_H