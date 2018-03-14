#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>

#include "Arena.h"
#include "missions/Mission.h"

enum TeamType{
    BLACK_BOX,
    CHEMICAL,
    DEBRIS,
    FIRE,
    WATER
};

enum CommandType{
    BASE,
    BONUS,
    END,
    NAVIGATED,
    START
};

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort(QSerialPortInfo& info, Arena& arena);
    QString& getTeamName();
    TeamType getTeamType();

signals:
    void newMessage(QString portName, QString message);
    void newName();
    void newCommand(QString portName, CommandType type, QString message);

private slots:
    void onReadyRead();

private:
    Arena& mArena;
    QString mTeamName;
    bool commandMode = false;
    bool running = false;
    TeamType mType;
    Mission *mission;
    QString mBuffer;

    void processCommand(QString buffer);
};

#endif // SERIALPORT_H
