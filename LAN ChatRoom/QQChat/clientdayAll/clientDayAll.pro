#-------------------------------------------------
#
# Project created by QtCreator 2023-03-29T14:59:24
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo3Single
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    usersocket.cpp \
    registordialog.cpp \
    CJsonObject.cpp \
    cJSON.c \
    msgbuilder.cpp \
    friendlist.cpp \
    msgmark.cpp \
    onefriend.cpp \
    chatdialog.cpp

HEADERS  += mainwidget.h \
    usersocket.h \
    registordialog.h \
    cJSON.h \
    CJsonObject.hpp \
    msgbuilder.h \
    friendlist.h \
    msgmark.h \
    onefriend.h \
    chatdialog.h

FORMS    += mainwidget.ui \
    registordialog.ui \
    friendlist.ui \
    msgmark.ui \
    onefriend.ui \
    chatdialog.ui

RESOURCES += \
    picture.qrc
