#include "multhreadserver.h"

mulThreadServer::mulThreadServer(QObject *parent) : QTcpServer(parent)
{

}

mulThreadServer::~mulThreadServer()
{

}

//当连接到来时 自动调用
void mulThreadServer::incomingConnection(qintptr descriptor)
{
    //发送信号给主窗口并且携带 套接字描述符
    emit newConnection_signal(descriptor);
}

