#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDateTime>

#include "Arena.h"
#include "missions/Mission.h"

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort(QSerialPortInfo& info, Arena& arena);
    QString& getTeamName();
    QString getTeamType();

signals:
    void newMessage(QString portName, QString message);
    void newName();
    void newCommand(QString portName, QString type, QString message);

private slots:
    void onReadyRead();
    void checkTime();

private:
    Arena& mArena;
    QString mTeamName;
    bool commandMode = false;
    bool running = false;
    QString mType;
    Mission *mission;
    QString mBuffer;
    QTimer *mTimeCheck;

    void processCommand(QString buffer);
};

#endif // SERIALPORT_H
