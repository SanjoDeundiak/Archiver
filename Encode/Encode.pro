#-------------------------------------------------
#
# Project created by QtCreator 2014-05-12T23:32:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Encode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainclass.cpp \
    dialog.cpp \
    Encoder.cpp

HEADERS  += mainwindow.h \
    mainclass.h \
    dialog.h \
    Encoder.h

FORMS    += mainwindow.ui \
    dialog.ui
QMAKE_CXXFLAGS += -std=gnu++0x
CONFIG += c++11
RC_ICONS = icon.ico
