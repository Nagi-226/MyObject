#ifndef MSGBUILDER_H
#define MSGBUILDER_H

#include "CJsonObject.hpp"
#include <QString>
#include <vector>

using namespace neb;
using namespace std;

class MsgBuilder
{
public:
    enum MsgType
    {
        registerUser,       //注册用户
        registerUserReturn, //注册用户服务器返回
        login,              //登录
        loginSucReturn,     //登录成功服务器返回
        loginLoseReturn,    //登录失败服务器返回
        sendMsg,            //发送聊天消息
        receiveMsg,         //收到聊天信息
        userOnline,         //用户上线
        userOffline,        //用户下线
        /*********群聊***********/
        createGroup,        //创建群聊
        createGroupReturn,  //创建群聊返回
        searchGroup,        //查找群聊
        searchGroupReturn,  //查找群聊返回
        joinGroup,          //加入群聊
        joinGroupReturn,    //加入群聊返回
        leaveGroup,         //离开群聊
        sendGroupMsg,       //发送群聊信息
        receiveGroupMsg,    //接收群聊信息
        userJoinGroup,      //用户加入群聊
        userLeaveGroup,     //用户离开群聊
    };
    struct UserData
    {
        int id;
        QString password;
        int headId;
        QString nickname;
        bool operator<(const UserData& other)const
        {
            return id < other.id;
        }
    };
    
    static int msgType(QString jsonStr);
    //注册用户    
    static QString buildRegisterUserMsg(QString password, int headId, QString nickname);
    static UserData parseRegisterUserMsg(QString jsonStr);
    //注册用户返回
    static QString buildRegisterUserReturnMsg(int id);
    static int parseRegisterUserReturnMsg(QString jsonStr);
    //登录
    static QString buildLoginMsg(int id, QString password);
    static UserData parseLoginMsg(QString jsonStr);
    //登录返回
    static QString buildLoginSucReturnMsg(MsgBuilder::UserData hostData, vector<UserData>& friends);
    static vector<UserData> parseLoginSucReturnMsg(MsgBuilder::UserData& hostData, QString jsonStr);
    static QString buildLoginLoseReturnMsg();
    //发送聊天数据
    static QString buildSendMsg(MsgBuilder::UserData from, MsgBuilder::UserData to, QString msg);
    static QString parseSendMsg(QString jsonStr, MsgBuilder::UserData& from, MsgBuilder::UserData& to);
    //收到聊天信息
    static QString buildReceiveMsg(MsgBuilder::UserData from, MsgBuilder::UserData to, QString msg);
    static QString parseReceiveMsg(QString jsonStr, MsgBuilder::UserData& from, MsgBuilder::UserData& to);
    //用户上线
    static QString buildUserOnline(MsgBuilder::UserData user);
    static UserData parseUserOnline(QString jsonStr);
    //用户下线
    static QString buildUserOffline(MsgBuilder::UserData user);
    static UserData parseUserOffline(QString jsonStr);
    //创建群聊
    static QString buildCreateGroup(QString name);
    static QString parseCreateGroup(QString jsonStr);
    //创建群聊返回
    static QString buildCreateGroupReturn(int groupId);
    static int parseCreateGroupReturn(QString jsonStr);
    //查找群聊
    static QString buildSearchGroup();
    //static int parseSearchGroup(QString jsonStr);
    //查找群聊返回
    static QString buildSearchGroupReturn(vector<int>& ids, vector<QString>& names);
    static void parseSearchGroupReturn(QString jsonStr, vector<int>& ids, vector<QString>& names);
    //加入群聊
    static QString buildJoinGroup(UserData user, int groupId);
    static UserData parseJoinGroup(QString jsonStr, int& groupId);
    //加入群聊返回
    static QString buildJoinGroupReturn(int groupId, vector<UserData>& users);
    static int parseJoinGroupReturn(QString jsonStr, vector<UserData>& users);
    //离开群聊
    static QString buildLeaveGroup(UserData user, int groupId);
    static int parseLeaveGroup(QString jsonStr, UserData& user);
    //发送群聊信息
    static QString buildSendGroupMsg(UserData from, int to, QString msg);
    static QString parseSendGroupMsg(QString jsonStr, UserData& from, int& to);
    //接收群聊信息
    static QString buildReceiveGroupMsg(int from, UserData to, QString msg);
    static QString parseReceiveGroupMsg(QString jsonStr, int& from, UserData& to);
    //用户加入群聊
    static QString buildUserJoinGroup(UserData user, int groupId);
    static void parseUserJoinGroup(QString jsonStr, UserData& user, int& groupId);
    //用户离开群聊
    static QString buildUserLeaveGroup(UserData user, int groupId);
    static void parseUserLeaveGroup(QString jsonStr, UserData& user, int& groupId);
};

#endif // MSGBUILDER_H
