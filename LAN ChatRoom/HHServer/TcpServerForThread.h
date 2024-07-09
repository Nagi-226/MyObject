#ifndef TCPSERVERFORTHREAD_H
#define TCPSERVERFORTHREAD_H

#include <QTcpServer>

class TcpServerForThread : public QTcpServer
{
    Q_OBJECT
public:
    TcpServerForThread(QObject* parent = NULL);
    ~TcpServerForThread();
    
signals:
    void newConnectionForThread(qintptr handle);
private:
    void incomingConnection(qintptr handle);//会在TcpServer收到客户端的连接时调用
};

#endif // TCPSERVERFORTHREAD_H
