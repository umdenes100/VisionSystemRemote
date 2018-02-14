QT += core serialport network websockets
#QT -= gui

CONFIG += c++11

TARGET = VisionSystemRemote
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Arena.cpp \
    Camera.cpp \
    Controller.cpp \
    SerialPort.cpp \
    SerialPortList.cpp \
    Server.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    Arena.h \
    Camera.h \
    Controller.h \
    Marker.h \
    Obstacle.h \
    Position.h \
    SerialPort.h \
    SerialPortList.h \
    Server.h

unix|win32: LIBS += -L/usr/local/lib -laruco -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
