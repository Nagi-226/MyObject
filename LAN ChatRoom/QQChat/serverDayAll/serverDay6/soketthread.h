#ifndef SOKETTHREAD_H
#define SOKETTHREAD_H

#include <QThread>
#include<QDebug>
#include<QTcpSocket>
#include<QByteArray>
#include<iostream>
#include<queue>//队列
#include<QMutex>//互斥锁
#include"msgbuilder.h"
#include"userdao.h"

using namespace std;

class SoketThread : public QThread
{
    Q_OBJECT
public:
    explicit SoketThread(qintptr descriptor,QObject *parent = 0);
    ~SoketThread();
    void  dealUserRegister(QString);//处理注册功能
    void dealUserLogin(QString);
    void addUserLoginSuc(vector<UserData>,UserData);

    void addUserOnLine(UserData);
    void addUserOffLine(UserData);

    void dealUserLoginSuc(QString);
    void  dealUserOnLine(QString);
    void dealUserOffLine(QString);

private:
    void run();
    qintptr sd;//记录套接字描述符
    QTcpSocket *socket;
    bool isRun;//标记连接是否断开 true 表示没断开
    bool isRead;//套接字是否有数据可读 true 可读
    queue<QString> msgQueue;//子线程收到的数据
    QMutex mutex;

    UserData udata;



signals:
    void loginSucSignal(UserData,SoketThread*);
    void  sigUserOffLine(UserData);


public slots:

    void on_readyRead_slot();//有数据到来触发
    void on_disconnected_slot();//连接断开触发

};

#endif // SOKETTHREAD_H
