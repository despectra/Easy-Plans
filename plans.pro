#-------------------------------------------------
#
# Project created by QtCreator 2015-07-02T09:02:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = plans
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    render2dwidget.cpp

HEADERS  += mainwindow.h \
    render2dwidget.h

FORMS    += mainwindow.ui