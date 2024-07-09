#-------------------------------------------------
#
# Project created by QtCreator 2021-01-28T08:47:45
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HHServer
TEMPLATE = app


SOURCES += main.cpp\
    CJsonObject.cpp \
    cJSON.c \
    MainWindow.cpp \
    MsgBuilder.cpp \
    NetworkThread.cpp \
    TcpServerForThread.cpp \
    UserDao.cpp \
    UserEntity.cpp

HEADERS  += \
    CJsonObject.hpp \
    cJSON.h \
    MainWindow.h \
    MsgBuilder.h \
    NetworkThread.h \
    Publicdef.h \
    TcpServerForThread.h \
    UserDao.h \
    UserEntity.h

FORMS    += mainwindow.ui

CONFIG += c++11
