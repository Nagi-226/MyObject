#ifndef USERSOCKET_H
#define USERSOCKET_H
#include<QTcpSocket>
#include"msgbuilder.h"
class userSocket:public QTcpSocket
{
public:

    ~userSocket();
    static userSocket *getInstance();

    //内联函数 套接字保存当前登录用户信息
    inline void setData(UserData data){udata =data; }

    //获取当前登录用户信息
    inline UserData getData(){return udata;}

private:
    userSocket();
    userSocket(const userSocket &);
     static userSocket*instance;
     UserData udata;//记录当前登录用户信息

};

#endif // USERSOCKET_H
