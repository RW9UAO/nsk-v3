#-------------------------------------------------
#
# Project created by QtCreator 2012-02-15T22:17:24
#
#-------------------------------------------------

QT       += core gui

TARGET = nsk_v3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    thread_485.cpp \
    dialogATV12.cpp \
    dialogCE303.cpp \
    thread_math.cpp \
    thread_I2C.cpp \
    dialogI2C.cpp \
    dialogPower.cpp \
    dialogAlarm.cpp \
    dialogNasos4.cpp \
    dialogNasos3.cpp \
    dialogNasos2.cpp \
    dialogNasos1.cpp \
    config.cpp \
    keypad.cpp \
    dialogService.cpp \
    dialogSetup.cpp

HEADERS  += mainwindow.h \
    thread_485.h \
    dialogATV12.h \
    data_struct.h \
    dialogCE303.h \
    thread_math.h \
    thread_I2C.h \
    dialogI2C.h \
    dialogPower.h \
    dialogAlarm.h \
    dialogNasos1.h \
    dialogNasos4.h \
    dialogNasos3.h \
    dialogNasos2.h \
    keypad.h \
    dialogService.h \
    dialogSetup.h

FORMS    += mainwindow.ui \
    dialogATV12.ui \
    dialogCE303.ui \
    dialogI2C.ui \
    dialogPower.ui \
    dialogAlarm.ui \
    dialogNasos1.ui \
    dialogNasos2.ui \
    dialogNasos3.ui \
    dialogNasos4.ui \
    keypad.ui \
    dialogService.ui \
    dialogSetup.ui

DEFINES += APP_VERSION=$$VERSION

OTHER_FILES += \
    config.txt

RESOURCES += \
    resource.qrc
