#ifndef HHTCPSOCKET_H
#define HHTCPSOCKET_H
#include <QTcpSocket>
#include "MsgBuilder.h"

class HHTcpSocket : public QTcpSocket
{
public:
    static HHTcpSocket* getInstance();
    static void releaseInstance();
    inline void setHostData(MsgBuilder::UserData data){hostData = data;}
    inline MsgBuilder::UserData getHostData(){return hostData;}
private:
    HHTcpSocket();
    ~HHTcpSocket();
    
    static HHTcpSocket* instance;
    MsgBuilder::UserData hostData;//本机用户的数据
};

#endif // HHTCPSOCKET_H
