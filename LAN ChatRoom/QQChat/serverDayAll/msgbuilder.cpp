#include "msgbuilder.h"

//注意，CJSON库不支持QT的任何类，只支持C++的string类型。
//将数据包userData转为串形式
QString MsgBuilder::buildUserRegisterMsg(UserData userData)
{
    CJsonObject obj;

    #if 0
    add函数
    以键值对形式添加
    参数1：都是key
    参数2：都是value
    返回值：添加是否成功，key不能有重复。
    #endif

            //添加了类型哦
    obj.Add("type", userRegister);//数据类型 type用常量 表达常量的方式：1.宏定义  2.枚举 所以头文件里封装枚举enum MsgType
    obj.Add("nickName", userData.nickName.toStdString());
    obj.Add("passwd", userData.passwd.toStdString());
    obj.Add("headId", userData.headId);

    std::string jsonStr = obj.ToString();
    return QString(jsonStr.c_str());
}


//将json串转为UserData形式 由userData带回
void MsgBuilder::parseUserRegisterMsg(QString jsonStr, UserData &userData)
{
    CJsonObject obj(jsonStr.toStdString());//解析json
    //获取昵称
    std::string nickName;

    #if 0
    get函数
    是从对象容器中根据key获得value。
    参数1都是key
    参数2是输出参数，C++使用引用作为输出参数，输出参数就是函数将结果通过参数返回给主调函数。
    返回值bool类型 猜猜什么意义？？真假 key 不一定能传对
    都表示获取数据是否成功。

    #endif
    obj.Get("nickName", nickName);
    userData.nickName = QString(nickName.c_str());

    //获取密码
    std::string passwd;
    obj.Get("passwd", passwd);
    userData.passwd = QString(passwd.c_str());
    //获取头像
    obj.Get("headId", userData.headId);

}

QString MsgBuilder::buildUserRegisterReturnMsg(UserData userData)
{
    CJsonObject obj;
    obj.Add("type", userRegisterReturn);//数据类型
    obj.Add("userId", userData.userId);
    obj.Add("passwd", userData.passwd.toStdString());
    obj.Add("nickName", userData.nickName.toStdString());
    obj.Add("headId", userData.headId);

    std::string jsonStr = obj.ToString();
    return QString(jsonStr.c_str());
}

void MsgBuilder::parseUserRegisterReturnMsg(QString jsonStr, UserData &userData)
{
    CJsonObject obj(jsonStr.toStdString());//解析json
    //获取账号
    obj.Get("userId", userData.userId);
    //获取密码
    std::string passwd;
    obj.Get("passwd", passwd);
    userData.passwd = QString(passwd.c_str());
    //获取昵称
    std::string nickName;
    obj.Get("nickName", nickName);
    userData.nickName = QString(nickName.c_str());
    //获取头像
    obj.Get("headId", userData.headId);
}

QString MsgBuilder::buildUserLoginMsg(UserData userData)
{
    CJsonObject obj;
    obj.Add("type", userLogin);//数据类型
    obj.Add("userId", userData.userId);
    obj.Add("passwd", userData.passwd.toStdString());

    std::string jsonStr = obj.ToString();
    return QString(jsonStr.c_str());
}

void MsgBuilder::parseUserLoginMsg(QString jsonStr, UserData &userData)
{
    CJsonObject obj(jsonStr.toStdString());//解析json
    //获取账号
    obj.Get("userId", userData.userId);
    //获取密码
    std::string passwd;
    obj.Get("passwd", passwd);
    userData.passwd = QString(passwd.c_str());
}

void MsgBuilder::parseUserLoginSucMsg(QString jsonStr, UserData &self, vector<UserData> &others)
{
    CJsonObject obj(jsonStr.toStdString());//解析json
    //获取账号
    obj.Get("userId", self.userId);
    //获取密码
//    std::string passwd;
//    obj.Get("passwd", passwd);
//    self.passwd = QString(passwd.c_str());
    //获取昵称
    std::string nickName;
    obj.Get("nickName", nickName);
    self.nickName = QString(nickName.c_str());
    //获取头像
    obj.Get("headId", self.headId);
    CJsonObject arr;
    obj.Get("others", arr);
    for(int i = 0;i < arr.GetArraySize();i++)
    {
        CJsonObject item;
        arr.Get(i, item);
        UserData ud;
        //获取账号
        item.Get("userId", ud.userId);
        //获取密码
//        std::string passwd;
//        item.Get("passwd", passwd);
//        ud.passwd = QString(passwd.c_str());
        //获取昵称
        std::string nickName;
        item.Get("nickName", nickName);
        ud.nickName = QString(nickName.c_str());
        //获取头像
        item.Get("headId", ud.headId);

        others.push_back(ud);
    }
}

QString MsgBuilder::buildUserLoginSucMsg(UserData self, vector<UserData> others)
{
    CJsonObject obj;
    obj.Add("type", userLoginSuc);//数据类型

    obj.Add("userId", self.userId);
    //obj.Add("passwd", self.passwd.toStdString());
    obj.Add("nickName", self.nickName.toStdString());
    obj.Add("headId", self.headId);

    CJsonObject arr;
    for(int i = 0;i < others.size();i++)
    {
        CJsonObject item;//数组中的一个元素对象，是对象容器
        item.Add("userId", others[i].userId);
        //item.Add("passwd", others[i].passwd.toStdString());
        item.Add("nickName", others[i].nickName.toStdString());
        item.Add("headId", others[i].headId);
        arr.Add(item);//将对象容器item加到数组容器中
    }

    //将数组容器添加到根容器中
    obj.Add("others", arr);

    std::string jsonStr = obj.ToString();
    qDebug()<<jsonStr.c_str();
    return QString(jsonStr.c_str());
}


//参数 上线的用户
QString MsgBuilder::buildUserOnlineMsg(UserData userData)
{
    CJsonObject obj;
    obj.Add("type", userOnline);//数据类型
    obj.Add("userId", userData.userId);
    obj.Add("nickName", userData.nickName.toStdString());
    obj.Add("headId", userData.headId);

    std::string jsonStr = obj.ToString();
    return QString(jsonStr.c_str());
}

//
void MsgBuilder::parseUserOnlineMsg(QString jsonStr, UserData &userData)
{
    CJsonObject obj(jsonStr.toStdString());//解析json
    //获取账号
    obj.Get("userId", userData.userId);
    //获取昵称
    std::string nickName;
    obj.Get("nickName", nickName);
    userData.nickName = QString(nickName.c_str());
    //获取头像
    obj.Get("headId", userData.headId);
}


QString MsgBuilder::buildUserLoginLoseMsg()
{
    CJsonObject obj;
    obj.Add("type", userLoginLose);//数据类型

    std::string jsonStr = obj.ToString();
    return QString(jsonStr.c_str());
}


QString MsgBuilder::buildUserOfflineMsg(UserData userData)
{
    CJsonObject obj;
    obj.Add("type", userOffline);//数据类型
    obj.Add("userId", userData.userId);
    obj.Add("nickName", userData.nickName.toStdString());
    obj.Add("headId", userData.headId);

    std::string jsonStr = obj.ToString();
    return QString(jsonStr.c_str());
}

void MsgBuilder::parseUserOfflineMsg(QString jsonStr, UserData &userData)
{
    CJsonObject obj(jsonStr.toStdString());//解析json
    //获取账号
    obj.Get("userId", userData.userId);
    //获取昵称
    std::string nickName;
    obj.Get("nickName", nickName);
    userData.nickName = QString(nickName.c_str());
    //获取头像
    obj.Get("headId", userData.headId);
}


QString MsgBuilder::buildUserSendMsg(UserData from, UserData to, QString msg)
{
    CJsonObject obj;
    obj.Add("type", userSendMsg);//数据类型
    obj.Add("fromUserId", from.userId);
    obj.Add("fromNickName", from.nickName.toStdString());
    obj.Add("fromHeadId", from.headId);

    obj.Add("toUserId", to.userId);
    obj.Add("toNickName", to.nickName.toStdString());
    obj.Add("toHeadId", to.headId);

    obj.Add("msg", msg.toStdString());

    std::string jsonStr = obj.ToString();
    return QString(jsonStr.c_str());
}

void MsgBuilder::parseUserSendMsg(QString jsonStr, UserData &from, UserData &to, QString &msg)
{
    CJsonObject obj(jsonStr.toStdString());//解析json
    //获取账号
    obj.Get("fromUserId", from.userId);
    //获取昵称
    std::string fromNickName;
    obj.Get("fromNickName", fromNickName);
    from.nickName = QString(fromNickName.c_str());
    //获取头像
    obj.Get("fromHeadId", from.headId);

    //获取账号
    obj.Get("toUserId", to.userId);
    //获取昵称
    std::string toNickName;
    obj.Get("toNickName", toNickName);
    to.nickName = QString(toNickName.c_str());
    //获取头像
    obj.Get("toHeadId", to.headId);

    std::string stdmsg;
    obj.Get("msg", stdmsg);
    msg = QString(stdmsg.c_str());
}

QString MsgBuilder::buildUserReceiveMsg(UserData from, UserData to, QString msg)
{
    CJsonObject obj;
    obj.Add("type", userReceiveMsg);//数据类型
    obj.Add("fromUserId", from.userId);
    obj.Add("fromNickName", from.nickName.toStdString());
    obj.Add("fromHeadId", from.headId);

    obj.Add("toUserId", to.userId);
    obj.Add("toNickName", to.nickName.toStdString());
    obj.Add("toHeadId", to.headId);

    obj.Add("msg", msg.toStdString());

    std::string jsonStr = obj.ToString();
    return QString(jsonStr.c_str());
}

void MsgBuilder::parseUserReceiveMsg(QString jsonStr, UserData &from, UserData &to, QString &msg)
{
    CJsonObject obj(jsonStr.toStdString());//解析json
    //获取账号
    obj.Get("fromUserId", from.userId);
    //获取昵称
    std::string fromNickName;
    obj.Get("fromNickName", fromNickName);
    from.nickName = QString(fromNickName.c_str());
    //获取头像
    obj.Get("fromHeadId", from.headId);

    //获取账号
    obj.Get("toUserId", to.userId);
    //获取昵称
    std::string toNickName;
    obj.Get("toNickName", toNickName);
    to.nickName = QString(toNickName.c_str());
    //获取头像
    obj.Get("toHeadId", to.headId);

    std::string stdmsg;
    obj.Get("msg", stdmsg);
    msg = QString(stdmsg.c_str());
}

int MsgBuilder::getType(QString jsonStr)
{
    CJsonObject obj(jsonStr.toStdString());//解析json
    int type;
    obj.Get("type", type);
    return type;
}
