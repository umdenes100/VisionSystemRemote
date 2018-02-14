#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include "Arena.h"

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort(QSerialPortInfo info, Arena& arena);

private slots:
    void onReadyRead();

private:
    Arena& mArena;
};

#endif // SERIALPORT_H