#-------------------------------------------------
#
# Project created by QtCreator 2023-03-30T10:24:35
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    multhreadserver.cpp \
    soketthread.cpp \
    msgbuilder.cpp \
    CJsonObject.cpp \
    cJSON.c \
    userdao.cpp

HEADERS  += mainwidget.h \
    multhreadserver.h \
    soketthread.h \
    msgbuilder.h \
    CJsonObject.hpp \
    cJSON.h \
    userdao.h
