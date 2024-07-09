#-------------------------------------------------
#
# Project created by QtCreator 2021-01-27T14:56:08
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HHClient
TEMPLATE = app


SOURCES += main.cpp\
    CJsonObject.cpp \
    cJSON.c \
    ChatDialog.cpp \
    ChatItem.cpp \
    ChatWidget.cpp \
    CreateGroupDialog.cpp \
    FriendItem.cpp \
    GroupChatDialog.cpp \
    HHTcpSocket.cpp \
    LoginDialog.cpp \
    MainWindow.cpp \
    MsgBuilder.cpp \
    RegisterDialog.cpp \
    SearchGroupDialog.cpp \
    MsgMarke.cpp

HEADERS  += \
    CJsonObject.hpp \
    cJSON.h \
    ChatDialog.h \
    ChatItem.h \
    ChatWidget.h \
    CreateGroupDialog.h \
    FriendItem.h \
    GroupChatDialog.h \
    HHTcpSocket.h \
    LoginDialog.h \
    MainWindow.h \
    MsgBuilder.h \
    Publicdef.h \
    RegisterDialog.h \
    SearchGroupDialog.h \
    MsgMarke.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    registerdialog.ui \
    frienditem.ui \
    chatdialog.ui \
    creategroupdialog.ui \
    groupchatdialog.ui \
    searchgroupdialog.ui \
    MsgMarke.ui

CONFIG += c++11

RESOURCES += \
    heads.qrc