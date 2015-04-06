#-------------------------------------------------
#
# Project created by QtCreator 2015-04-04T09:57:41
#
#-------------------------------------------------

QT       += core gui sql network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ATPProject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    userinfo.cpp

HEADERS  += mainwindow.h \
    userinfo.h

FORMS    += mainwindow.ui
