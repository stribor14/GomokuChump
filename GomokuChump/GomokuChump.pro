#-------------------------------------------------
#
# Project created by QtCreator 2017-01-13T14:49:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GomokuChump
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    playscene.cpp \
    playingboard.cpp

HEADERS  += mainwindow.h \
    playscene.h \
    playingboard.h

FORMS    += mainwindow.ui

LIBS += -lyaml-cpp
