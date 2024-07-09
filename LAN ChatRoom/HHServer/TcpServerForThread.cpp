#include "TcpServerForThread.h"

TcpServerForThread::TcpServerForThread(QObject *parent):
    QTcpServer(parent)
{
    
}

TcpServerForThread::~TcpServerForThread()
{
    
}

void TcpServerForThread::incomingConnection(qintptr handle)
{
    emit newConnectionForThread(handle);//默认在函数中会根据handle创建一个QTcpSocket对象，但是我们不直接创建
    //最终要把这个参数传给线程， 在线程中创建QTcpSocket对象
}

