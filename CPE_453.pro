#-------------------------------------------------
#
# Project created by QtCreator 2015-01-18T08:42:04
#
#-------------------------------------------------

QT       += core gui sql network



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPE_453
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    trainDisplayInfo.cpp \
    sqlServer.cpp

HEADERS  += mainwindow.h \
    trainDisplayInfo.h \
    sqlServer.h

FORMS    += \
    mainwindow.ui
