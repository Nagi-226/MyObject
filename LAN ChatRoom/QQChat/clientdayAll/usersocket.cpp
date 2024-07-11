#include "usersocket.h"

userSocket * userSocket::instance = new userSocket;
userSocket::userSocket()
{

}

userSocket::userSocket(const userSocket & other)
{

}

userSocket::~userSocket()
{

}

//
userSocket * userSocket::getInstance()
{
    return instance;
}

