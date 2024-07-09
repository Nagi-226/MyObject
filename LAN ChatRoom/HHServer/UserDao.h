#ifndef USERDAO_H
#define USERDAO_H

#include <QSqlDatabase>
#include "UserEntity.h"

//dao   data   abstract  object
class UserDao
{
public:
    static UserDao* getInstance();
    static void releaseInstance();
    
    int insertUser(UserEntity& e);//获得插入数据的id
    bool isPasswordRight(UserEntity& e);//登录成功会把用户名写入参数
private:
    UserDao();
    ~UserDao();
    static UserDao *instance;
    QSqlDatabase db;//qt中用来打开和关闭数据库的类
};

#endif // USERDAO_H
