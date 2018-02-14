#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include "Arena.h"
#include "Server.h"

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort(QSerialPortInfo& info, Arena& arena);

signals:
    void transmit(QString buffer);

private slots:
    void onReadyRead();

private:
    Arena& mArena;

    void processCommand(QString buffer);
};

#endif // SERIALPORT_H
