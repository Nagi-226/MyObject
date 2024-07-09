#include "HHTcpSocket.h"

HHTcpSocket* HHTcpSocket::instance = 0;

HHTcpSocket *HHTcpSocket::getInstance()
{
    if(instance == 0)
    {
        instance = new HHTcpSocket;
    }
    return instance;
}

void HHTcpSocket::releaseInstance()
{
    if(instance != 0)
    {
        instance->close();
        delete instance;
    }
}

HHTcpSocket::HHTcpSocket()
{
    
}

HHTcpSocket::~HHTcpSocket()
{
    
}

