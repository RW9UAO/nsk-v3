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
    dialogNasos1.cpp \
    config.cpp \
    keypad.cpp \
    dialogService.cpp \
    dialogSetup.cpp \
    dialogSetupGeneral.cpp \
    dialogCalibr.cpp \
    dialogNasosParamerer.cpp

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
    keypad.h \
    dialogService.h \
    dialogSetup.h \
    dialogSetupGeneral.h \
    dialogCalibr.h \
    dialogNasosParameter.h

FORMS    += mainwindow.ui \
    dialogATV12.ui \
    dialogCE303.ui \
    dialogI2C.ui \
    dialogPower.ui \
    dialogAlarm.ui \
    dialogNasos1.ui \
    keypad.ui \
    dialogService.ui \
    dialogSetup.ui \
    dialogSetupGeneral.ui \
    dialogCalibr.ui \
    dialogNasosParameter.ui

DEFINES += APP_VERSION=$$VERSION

OTHER_FILES += \
    config.txt

RESOURCES += \
    resource.qrc
