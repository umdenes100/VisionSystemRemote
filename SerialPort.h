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
    QString& getTeamName();

signals:
    void transmit(QString teamName, QString buffer);

private slots:
    void onReadyRead();

private:
    Arena& mArena;
    QString mTeamName;

    void processCommand(QString buffer);
};

#endif // SERIALPORT_H
