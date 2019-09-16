#-------------------------------------------------
#
# Project created by QtCreator 2018-03-02T14:56:57
#
#-------------------------------------------------

QT       += core gui serialport network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VisionSystemRemote
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Arena.cpp \
    Camera.cpp \
    Controller.cpp \
    Server.cpp \
    missions/BlackBoxMission.cpp \
    missions/DataMission.cpp \
    missions/DebrisMission.cpp \
    missions/FireMission.cpp \
    missions/WaterMission.cpp \
    ConnectionList.cpp \
    Connection.cpp

HEADERS += \
        mainwindow.h \
    Arena.h \
    Camera.h \
    Controller.h \
    Marker.h \
    Obstacle.h \
    Position.h \
    Server.h \
    missions/BlackBoxMission.h \
    missions/DataMission.h \
    missions/DebrisMission.h \
    missions/FireMission.h \
    missions/Mission.h \
    missions/WaterMission.h \
    arucomarker.h \
    ConnectionList.h \
    Connection.h

FORMS += \
        mainwindow.ui

unix|win32: LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_aruco -lopencv_imgcodecs -lopencv_imgproc

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

