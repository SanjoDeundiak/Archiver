#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T02:12:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Decode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Decoder.cpp

HEADERS  += mainwindow.h \
    Decoder.h

FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=gnu++0x
CONFIG += c++11
RC_ICONS = icon.ico
