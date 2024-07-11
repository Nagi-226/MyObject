#ifndef MULTHREADSERVER_H
#define MULTHREADSERVER_H


#include<QTcpServer>

class mulThreadServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit mulThreadServer(QObject *parent = 0);
    ~mulThreadServer();

protected:
    void incomingConnection(qintptr descriptor);
signals:
    void newConnection_signal(qintptr descriptor);

public slots:
};

#endif // MULTHREADSERVER_H
