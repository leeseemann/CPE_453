#-------------------------------------------------
#
# Project created by QtCreator 2015-01-18T08:42:04
#
#-------------------------------------------------

QT       += core gui sql network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPE_453
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    trainDisplayInfo.cpp \
    userinfo.cpp

HEADERS  += mainwindow.h \
    trainDisplayInfo.h \
    userinfo.h

FORMS    += \
    mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
