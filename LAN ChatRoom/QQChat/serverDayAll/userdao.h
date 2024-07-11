#ifndef USERDAO_H
#define USERDAO_H

#include<QSqlDatabase>
#include<QSqlQuery>
#include<QVariant>
#include<QSqlRecord>

//用于数据库操作
struct UserEntity
{
    //每个成员对应表中的一个字段，主要用于数据的传递
    int userId;
    QString nickName;
    QString passwd;
    int headId;
};

//单例模式
class UserDao
{
public:

    ~UserDao();//关闭数据库
    static UserDao* getInstance();

    //返回值表示sql执行是否成功
       bool insertUser(UserEntity& e);//用于注册
       bool selectUser(UserEntity& e, bool& ok);//用于登录

private:
     UserDao();//打开数据库
     UserDao(const UserDao&);
     static UserDao *instance;
     QSqlDatabase db;

};

#endif // USERDAO_H
