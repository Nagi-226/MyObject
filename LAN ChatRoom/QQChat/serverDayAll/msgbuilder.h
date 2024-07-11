#ifndef MSGBUILDER_H
#define MSGBUILDER_H

#include<QString>
#include"cJSON.h"
#include"CJsonObject.hpp"
#include<iostream>
#include<vector>
#include<QDebug>

using namespace neb;
using namespace std;


//表示用户基本信息的类，因为没有逻辑可写，是一个纯数据类型，所以使用struct
struct UserData
{
    int userId;//账号
    QString passwd;//密码
    QString nickName;//昵称
    int headId;//头像

    //自定义类作为map的key使用，必须重载<运算符，并且要求是常函数
    //常对象只能调用常函数，常函数中的成员变量不能修改
    bool operator<(const UserData& other) const
    {
        return userId<other.userId;
    }
};

/*
    MsgBuilder要对CJson进行封装，主要是静态成员函数，之所以使用静态，因为这里不需要成员变量，只是一些函数，函数之间的功能相对独立
    构建和解析json的成员函数，也可以写成全局的，写在类中更加有利于封装。
    这个类客户端和服务端都要使用，而且要同步。
    */
class MsgBuilder
{
public:
    /*
               常量起名，可以使用宏定义，也可以使用枚举。
               宏定义表达不出常量之间的逻辑关系，而枚举可以，一个枚举中的所有成员一定是有逻辑关系的。
           */
    enum MsgType
    {
        userRegister,//注册
        userLogin,//登录
        userRegisterReturn,//注册返回，返回新用户的id
        userLoginSuc,
        userLoginLose,
        userOnline,
        userOffline,
        userSendMsg,
        userReceiveMsg,
    };
    //这些函数都是对json的构建和解析 所以封装到类里

    //构造：结构体转为json串 并加入type类型
    //解析：从json串中获取结构体成员

    //构建和解析  用户注册的json串
    static QString buildUserRegisterMsg(UserData userData);
    static void parseUserRegisterMsg(QString jsonStr, UserData& userData);

    //构建和解析  用户注册返回json串
    static QString buildUserRegisterReturnMsg(UserData userData);
    static void parseUserRegisterReturnMsg(QString jsonStr, UserData& userData);

    //构建和解析  用户登录json串
    static QString buildUserLoginMsg(UserData userData);
    static void parseUserLoginMsg(QString jsonStr, UserData& userData);

    //构建和解析  登录成功
    static QString buildUserLoginSucMsg(UserData self, vector<UserData> others);
    static void parseUserLoginSucMsg(QString jsonStr, UserData& self, vector<UserData>& others);

    //构建和解析  用户上线json串
    static QString buildUserOnlineMsg(UserData userData);//参数是上线的用户
    static void parseUserOnlineMsg(QString jsonStr, UserData& userData);

    //构建和解析  用户下线json串
    static QString buildUserOfflineMsg(UserData userData);
    static void parseUserOfflineMsg(QString jsonStr, UserData& userData);

    //构建和解析  用户发送私聊json串
    static QString buildUserSendMsg(UserData from, UserData to, QString msg);
    static void parseUserSendMsg(QString jsonStr, UserData& from,
                                 UserData& to, QString& msg);

    //构建和解析  用户接收私聊json串
    static QString buildUserReceiveMsg(UserData from, UserData to, QString msg);
      static void parseUserReceiveMsg(QString jsonStr, UserData& from,UserData& to, QString& msg);


    //构建登录失败
    static QString buildUserLoginLoseMsg();



    //获得json串的类型
    static int getType(QString jsonStr);
};

#endif // MSGBUILDER_H
