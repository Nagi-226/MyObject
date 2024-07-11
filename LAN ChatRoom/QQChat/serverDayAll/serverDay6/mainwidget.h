#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include"multhreadserver.h"
#include<QDebug>
#include<QHostAddress>
#include"soketthread.h"
#include"msgbuilder.h"


class mainWidget : public QWidget
{
    Q_OBJECT

public:
    mainWidget(QWidget *parent = 0);
    ~mainWidget();
    void on_loginSucSignal_slot(UserData,SoketThread*);
   void  deal_newconnection_slot(qintptr descriptor);
   void on_sigUserOffLine(UserData);
private:
   mulThreadServer *mtServer;
   SoketThread *std;
   map<UserData,SoketThread*> users;//管理所有在线用户
};

#endif // MAINWIDGET_H
