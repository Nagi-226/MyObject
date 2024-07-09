#include "MsgBuilder.h"

int MsgBuilder::msgType(QString jsonStr)
{
    int type;
    CJsonObject jsonObj(jsonStr.toStdString());
    if(jsonObj.Get("type", type))
    {
        return type;
    }
    else
    {
        return -1;
    }
}

QString MsgBuilder::buildRegisterUserMsg(QString password, int headId, QString nickname)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", registerUser);
    jsonObj.Add("password", password.toStdString());
    jsonObj.Add("headId", headId);
    jsonObj.Add("nickname", nickname.toStdString());
    QString retData(jsonObj.ToString().c_str());
    return retData;
}

MsgBuilder::UserData MsgBuilder::parseRegisterUserMsg(QString jsonStr)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    UserData data;
    std::string stdStr;
    jsonObj.Get("password", stdStr);
    data.password = QString(stdStr.c_str());
    jsonObj.Get("headId", data.headId);
    jsonObj.Get("nickname", stdStr);
    data.nickname = QString(stdStr.c_str());
    return data;
}

QString MsgBuilder::buildRegisterUserReturnMsg(int id)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", registerUserReturn);
    jsonObj.Add("id", id);
    QString retData(jsonObj.ToString().c_str());
    return retData;
}

int MsgBuilder::parseRegisterUserReturnMsg(QString jsonStr)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    int id;
    jsonObj.Get("id", id);
    return id;
}

QString MsgBuilder::buildLoginMsg(int id, QString password)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", login);
    jsonObj.Add("id", id);
    jsonObj.Add("password", password.toStdString());
    QString retData(jsonObj.ToString().c_str());
    return retData;
}

MsgBuilder::UserData MsgBuilder::parseLoginMsg(QString jsonStr)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    UserData data;
    std::string stdStr;
    jsonObj.Get("password", stdStr);
    data.password = QString(stdStr.c_str());
    jsonObj.Get("id", data.id); 
    return data;
}

QString MsgBuilder::buildLoginSucReturnMsg(MsgBuilder::UserData hostData, vector<MsgBuilder::UserData> &friends)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", loginSucReturn);
    CJsonObject friendsArr;
    for(int i = 0;i < friends.size();i++)
    {
        CJsonObject friendObj;
        friendObj.Add("id", friends[i].id);
        friendObj.Add("headId", friends[i].headId);
        friendObj.Add("nickname", friends[i].nickname.toStdString());     
        friendsArr.Add(friendObj);
    }
    jsonObj.Add("friends", friendsArr);
    jsonObj.Add("hostId",hostData.id);
    jsonObj.Add("hostHeadId",hostData.headId);
    jsonObj.Add("hostNickname",hostData.nickname.toStdString());
    QString retData(jsonObj.ToString().c_str());
    return retData;
}

vector<MsgBuilder::UserData> MsgBuilder::parseLoginSucReturnMsg(MsgBuilder::UserData& hostData, QString jsonStr)
{
    vector<UserData> friends;
    CJsonObject jsonObj(jsonStr.toStdString());
    CJsonObject friendsArr;
    jsonObj.Get("friends", friendsArr);
    for(int i = 0;i < friendsArr.GetArraySize();i++)
    {
        UserData data;
        CJsonObject friendObj = friendsArr[i];
        friendObj.Get("id", data.id);
        friendObj.Get("headId", data.headId);
        string nickname;
        friendObj.Get("nickname", nickname);
        data.nickname = QString(nickname.c_str());
        friends.push_back(data);
    }
    jsonObj.Get("hostId", hostData.id);
    jsonObj.Get("hostHeadId", hostData.headId);
    string hostNickname;
    jsonObj.Get("hostNickname", hostNickname);
    hostData.nickname = QString(hostNickname.c_str());
    return friends;
}

QString MsgBuilder::buildLoginLoseReturnMsg()
{
    CJsonObject jsonObj;
    jsonObj.Add("type", loginLoseReturn);
    QString retData(jsonObj.ToString().c_str());
    return retData;
}

QString MsgBuilder::buildSendMsg(MsgBuilder::UserData from, MsgBuilder::UserData to, QString msg)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", sendMsg);
    jsonObj.Add("fromId", from.id);
    jsonObj.Add("toId", to.id);
    jsonObj.Add("msg", msg.toStdString());
    QString ret(jsonObj.ToString().c_str());
    return ret;
}

QString MsgBuilder::parseSendMsg(QString jsonStr, MsgBuilder::UserData &from, MsgBuilder::UserData &to)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    jsonObj.Get("fromId", from.id);
    jsonObj.Get("toId", to.id);
    string msg;
    jsonObj.Get("msg", msg);
    return QString(msg.c_str());
}

QString MsgBuilder::buildReceiveMsg(MsgBuilder::UserData from, MsgBuilder::UserData to, QString msg)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", receiveMsg);
    jsonObj.Add("fromId", from.id);
    jsonObj.Add("toId", to.id);
    jsonObj.Add("msg", msg.toStdString());
    QString ret(jsonObj.ToString().c_str());
    return ret;
}

QString MsgBuilder::parseReceiveMsg(QString jsonStr, MsgBuilder::UserData &from, MsgBuilder::UserData &to)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    jsonObj.Get("fromId", from.id);
    jsonObj.Get("toId", to.id);
    string msg;
    jsonObj.Get("msg", msg);
    return QString(msg.c_str());
}

QString MsgBuilder::buildUserOnline(MsgBuilder::UserData user)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", userOnline);
    jsonObj.Add("id", user.id);
    jsonObj.Add("headId", user.headId);
    jsonObj.Add("nickname", user.nickname.toStdString());
    QString ret(jsonObj.ToString().c_str());
    return ret;
}

MsgBuilder::UserData MsgBuilder::parseUserOnline(QString jsonStr)
{
    UserData user;
    CJsonObject jsonObj(jsonStr.toStdString());
    jsonObj.Get("id", user.id);
    jsonObj.Get("headId", user.headId);
    string nickname;
    jsonObj.Get("nickname", nickname);
    user.nickname = QString(nickname.c_str());
    return user;
}

QString MsgBuilder::buildUserOffline(MsgBuilder::UserData user)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", userOffline);
    jsonObj.Add("id", user.id);
    jsonObj.Add("headId", user.headId);
    jsonObj.Add("nickname", user.nickname.toStdString());
    QString ret(jsonObj.ToString().c_str());
    return ret;
}

MsgBuilder::UserData MsgBuilder::parseUserOffline(QString jsonStr)
{
    UserData user;
    CJsonObject jsonObj(jsonStr.toStdString());
    jsonObj.Get("id", user.id);
    jsonObj.Get("headId", user.headId);
    string nickname;
    jsonObj.Get("nickname", nickname);
    user.nickname = QString(nickname.c_str());
    return user;
}

QString MsgBuilder::buildCreateGroup(QString name)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", createGroup);
    jsonObj.Add("name", name.toStdString());
    return QString(jsonObj.ToString().c_str());
}

QString MsgBuilder::parseCreateGroup(QString jsonStr)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    string stdName;
    jsonObj.Get("name", stdName);
    return QString(stdName.c_str());
}

QString MsgBuilder::buildCreateGroupReturn(int groupId)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", createGroupReturn);
    jsonObj.Add("groupId", groupId);
    return QString(jsonObj.ToString().c_str());
}

int MsgBuilder::parseCreateGroupReturn(QString jsonStr)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    int groupId;
    jsonObj.Get("groupId", groupId);
    return groupId;
}

QString MsgBuilder::buildSearchGroup()
{
    CJsonObject jsonObj;
    jsonObj.Add("type", searchGroup);
    return QString(jsonObj.ToString().c_str());
}

//int MsgBuilder::parseSearchGroup(QString jsonStr)
//{
//    CJsonObject jsonObj(jsonStr.toStdString());
//    int groupId;
//    jsonObj.Get("groupId", groupId);
//    return groupId;
//}

QString MsgBuilder::buildSearchGroupReturn(vector<int> &ids, vector<QString> &names)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", searchGroupReturn);
    CJsonObject jsonArr;
    for(int i = 0;i < ids.size();i++)
    {
        CJsonObject group;
        group.Add("id", ids[i]);
        group.Add("name", names[i].toStdString());
        jsonArr.Add(group);
    }
    jsonObj.Add("groups", jsonArr);
    return QString(jsonObj.ToString().c_str());
}

void MsgBuilder::parseSearchGroupReturn(QString jsonStr, vector<int> &ids, vector<QString> &names)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    CJsonObject jsonArr;
    jsonObj.Get("groups", jsonArr);
    for(int i = 0;i < jsonArr.GetArraySize();i++)
    {
        CJsonObject group;
        jsonArr.Get(i, group);
        int id;
        group.Get("id",id);
        ids.push_back(id);
        string stdName;
        group.Get("name", stdName);
        names.push_back(QString(stdName.c_str()));
    }
}

QString MsgBuilder::buildJoinGroup(MsgBuilder::UserData user, int groupId)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", joinGroup);
    jsonObj.Add("id", user.id);
    jsonObj.Add("headId", user.headId);
    jsonObj.Add("nickname", user.nickname.toStdString());
    jsonObj.Add("groupId", groupId);
    return QString(jsonObj.ToString().c_str());
}

MsgBuilder::UserData MsgBuilder::parseJoinGroup(QString jsonStr, int& groupId)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    UserData user;
    jsonObj.Get("id", user.id);
    jsonObj.Get("headId", user.headId);
    string stdNickname;
    jsonObj.Get("nickname", stdNickname);
    user.nickname = QString(stdNickname.c_str());
    jsonObj.Get("groupId", groupId);
    return user;
}

QString MsgBuilder::buildJoinGroupReturn(int groupId, vector<MsgBuilder::UserData> &users)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", joinGroupReturn);
    jsonObj.Add("groupId", groupId);
    CJsonObject jsonArr;
    for(int i = 0;i < users.size();i++)
    {
        CJsonObject user;
        user.Add("id", users[i].id);
        user.Add("headId", users[i].headId);
        user.Add("nickname", users[i].nickname.toStdString());
        jsonArr.Add(user);
    }
    jsonObj.Add("users", jsonArr);
    return QString(jsonObj.ToString().c_str());
}

int MsgBuilder::parseJoinGroupReturn(QString jsonStr, vector<MsgBuilder::UserData> &users)
{
    CJsonObject jsonObj(jsonStr.toStdString());
    int groupId;
    jsonObj.Get("groupId", groupId);
    CJsonObject jsonArr;
    jsonObj.Get("users", jsonArr);
    for(int i = 0;i < jsonArr.GetArraySize();i++)
    {
        CJsonObject jsonUser;
        jsonArr.Get(i, jsonUser);
        UserData user;
        jsonUser.Get("id", user.id);
        jsonUser.Get("headId", user.headId);
        string stdNickname;
        jsonUser.Get("nickname", stdNickname);
        user.nickname = QString(stdNickname.c_str());
        users.push_back(user);
    }
    
    return groupId;
}

QString MsgBuilder::buildLeaveGroup(MsgBuilder::UserData user, int groupId)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", leaveGroup);
    jsonObj.Add("groupId", groupId);
    jsonObj.Add("id", user.id);
    jsonObj.Add("headId", user.headId);
    jsonObj.Add("nickname", user.nickname.toStdString());
    return QString(jsonObj.ToString().c_str());
}

int MsgBuilder::parseLeaveGroup(QString jsonStr, MsgBuilder::UserData &user)
{
    CJsonObject jsonObj(jsonStr.toStdString());

    jsonObj.Get("id", user.id);
    jsonObj.Get("headId", user.headId);
    string stdNickname;
    jsonObj.Get("nickname", stdNickname);
    user.nickname = QString(stdNickname.c_str());
    
    int groupId;
    jsonObj.Get("groupId", groupId);
    return groupId;
}

QString MsgBuilder::buildSendGroupMsg(MsgBuilder::UserData from, int to, QString msg)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", sendGroupMsg);
    jsonObj.Add("groupId", to);
    jsonObj.Add("id", from.id);
    jsonObj.Add("headId", from.headId);
    jsonObj.Add("nickname", from.nickname.toStdString());
    jsonObj.Add("msg", msg.toStdString());
    return QString(jsonObj.ToString().c_str());
}

QString MsgBuilder::parseSendGroupMsg(QString jsonStr, MsgBuilder::UserData &from, int &to)
{
    CJsonObject jsonObj(jsonStr.toStdString());

    jsonObj.Get("id", from.id);
    jsonObj.Get("headId", from.headId);
    string stdNickname;
    jsonObj.Get("nickname", stdNickname);
    from.nickname = QString(stdNickname.c_str());
    
    jsonObj.Get("groupId", to);
    
    string stdMsg;
    jsonObj.Get("msg", stdMsg);
    return QString(stdMsg.c_str());
}

QString MsgBuilder::buildReceiveGroupMsg(int from, MsgBuilder::UserData to, QString msg)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", receiveGroupMsg);
    jsonObj.Add("groupId", from);
    jsonObj.Add("id", to.id);
    jsonObj.Add("headId", to.headId);
    jsonObj.Add("nickname", to.nickname.toStdString());
    jsonObj.Add("msg", msg.toStdString());
    return QString(jsonObj.ToString().c_str());
}

QString MsgBuilder::parseReceiveGroupMsg(QString jsonStr, int &from, MsgBuilder::UserData &to)
{
    CJsonObject jsonObj(jsonStr.toStdString());

    jsonObj.Get("id", to.id);
    jsonObj.Get("headId", to.headId);
    string stdNickname;
    jsonObj.Get("nickname", stdNickname);
    to.nickname = QString(stdNickname.c_str());
    
    jsonObj.Get("groupId", from);
    
    string stdMsg;
    jsonObj.Get("msg", stdMsg);
    return QString(stdMsg.c_str());
}

QString MsgBuilder::buildUserJoinGroup(MsgBuilder::UserData user, int groupId)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", userJoinGroup);
    jsonObj.Add("groupId", groupId);
    jsonObj.Add("id", user.id);
    jsonObj.Add("headId", user.headId);
    jsonObj.Add("nickname", user.nickname.toStdString());
    return QString(jsonObj.ToString().c_str());
}

void MsgBuilder::parseUserJoinGroup(QString jsonStr, MsgBuilder::UserData &user, int &groupId)
{
    CJsonObject jsonObj(jsonStr.toStdString());

    jsonObj.Get("id", user.id);
    jsonObj.Get("headId", user.headId);
    string stdNickname;
    jsonObj.Get("nickname", stdNickname);
    user.nickname = QString(stdNickname.c_str());
    
    jsonObj.Get("groupId", groupId);   
}

QString MsgBuilder::buildUserLeaveGroup(MsgBuilder::UserData user, int groupId)
{
    CJsonObject jsonObj;
    jsonObj.Add("type", userLeaveGroup);
    jsonObj.Add("groupId", groupId);
    jsonObj.Add("id", user.id);
    jsonObj.Add("headId", user.headId);
    jsonObj.Add("nickname", user.nickname.toStdString());
    return QString(jsonObj.ToString().c_str());
}

void MsgBuilder::parseUserLeaveGroup(QString jsonStr, MsgBuilder::UserData &user, int &groupId)
{
    CJsonObject jsonObj(jsonStr.toStdString());

    jsonObj.Get("id", user.id);
    jsonObj.Get("headId", user.headId);
    string stdNickname;
    jsonObj.Get("nickname", stdNickname);
    user.nickname = QString(stdNickname.c_str());
    
    jsonObj.Get("groupId", groupId);   
}


