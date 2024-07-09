#include "NetworkThread.h"
#include <QDebug>
#include "UserDao.h"
#include "UserEntity.h"


NetWorkThread::NetWorkThread()
{
    
}

NetWorkThread::NetWorkThread(qintptr handle, QObject *parent):
    QThread(parent)
{
    //自定义信号带自定义参数类型，需要注册参数类型
    //qRegisterMetaType<MsgBuilder::UserData>("MsgBuilder::UserData");
        
    //这个socket还可以在线程的逻辑中直接创建
    socket = new QTcpSocket;
    socket->setSocketDescriptor(handle);
    test = socket->peerPort();
    socket->moveToThread(this);//构造函数的执行是在主线程，所以这个需要把自己创建的QTcpSocket对象移交给子线程
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));   
}

NetWorkThread::~NetWorkThread()
{
    
}

void NetWorkThread::addLoginSuccessInfo(MsgBuilder::UserData user, vector<MsgBuilder::UserData> &friends)
{
    QString ret = MsgBuilder::buildLoginSucReturnMsg(user, friends);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::addReceiveMsgInfo(MsgBuilder::UserData from, MsgBuilder::UserData to, QString msg)
{
    QString ret = MsgBuilder::buildReceiveMsg(from, to, msg);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::addUserOnlineInfo(MsgBuilder::UserData user)
{
    QString ret = MsgBuilder::buildUserOnline(user);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::addUserOfflineInfo(MsgBuilder::UserData user)
{
    QString ret = MsgBuilder::buildUserOffline(user);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::addCreateGroupReturnInfo(int groupId)
{
    QString ret = MsgBuilder::buildCreateGroupReturn(groupId);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::addSearchGroupReturnInfo(vector<int>& ids, vector<QString>& names)
{
    QString ret = MsgBuilder::buildSearchGroupReturn(ids, names);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::addJoinGroupReturnInfo(int groupId, vector<MsgBuilder::UserData> &groupFriends)
{  
    QString ret = MsgBuilder::buildJoinGroupReturn(groupId, groupFriends);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::addUserJoinGroupInfo(MsgBuilder::UserData user, int groupId)
{
    QString ret = MsgBuilder::buildUserJoinGroup(user, groupId);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::addUserLeaveGroupInfo(MsgBuilder::UserData user, int groupId)
{
    QString ret = MsgBuilder::buildUserLeaveGroup(user, groupId);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::addReceiveGroupMsgInfo(MsgBuilder::UserData to, int from, QString msg)
{
    QString ret = MsgBuilder::buildReceiveGroupMsg(from, to, msg);
    mutex.lock();
    msgQueue.push(ret);
    mutex.unlock();
}

void NetWorkThread::run()
{
    while(true)
    {
        if(isDisconnected)//断开连接
        {
            break;
        }
        if(isRead)//读取数据
        {
            isRead = false;
            while(socket->bytesAvailable()>0)//可能接收到不只一帧数据
            {
                QByteArray byteArray = socket->readAll();
                QString data = QString::fromLocal8Bit(byteArray);
                mutex.lock();
                msgQueue.push(data);
                mutex.unlock();
            }
        }
        socket->waitForReadyRead(10);//使用waitForReadyRead做延时，防止因为循环速度过快不能收到socket信息
        mutex.lock();
        if(msgQueue.empty())//如果消息队列是空，循环下面的代码就不执行了
        {
            mutex.unlock();
            continue;
        }
        QString data = msgQueue.front();  //消息出队     
        msgQueue.pop();
        mutex.unlock();
        switch(MsgBuilder::msgType(data))
        {
        case MsgBuilder::registerUser:
            dealRegisterUser(data);
            break;
        case MsgBuilder::login:
            dealLogin(data);
            break;
        case MsgBuilder::loginSucReturn:
            dealLoginSuc(data);
            break;
        case MsgBuilder::sendMsg:
            dealSendMsg(data);
            break;
        case MsgBuilder::receiveMsg:
            dealReceiveMsg(data);
            break;
        case MsgBuilder::userOnline:
            dealUserOnline(data);
            break;
        case MsgBuilder::userOffline:
            dealUserOffline(data);
            break;
        case MsgBuilder::createGroup:
            dealCreateGroup(data);
            break;
        case MsgBuilder::createGroupReturn:
            dealCreateGroupReturn(data);
            break;
        case MsgBuilder::searchGroup:
            dealSearchGroup(data);
            break;
        case MsgBuilder::searchGroupReturn:
            dealSearchGroupReturn(data);
            break;
        case MsgBuilder::joinGroup:
            dealJoinGroup(data);
            break;
        case MsgBuilder::joinGroupReturn:
            dealJoinGroupReturn(data);
            break;
        case MsgBuilder::userJoinGroup:
            dealUserJoinGroup(data);
            break;
        case MsgBuilder::leaveGroup:
            dealLeaveGroup(data);
            break;
        case MsgBuilder::userLeaveGroup:
            dealUserLeaveGroup(data);
            break;
        case MsgBuilder::sendGroupMsg:
            dealSendGroupMsg(data);
            break;
        case MsgBuilder::receiveGroupMsg:
            dealReceiveGroupMsg(data);
            break;
        }
    }
    emit userOffline(user);
}

void NetWorkThread::dealRegisterUser(QString jsonStr)
{
    //解析数据
    MsgBuilder::UserData data = MsgBuilder::parseRegisterUserMsg(jsonStr);
    //插入数据库
    UserEntity ue;
    ue.password = data.password;
    ue.headId = data.headId;
    ue.nickname = data.nickname;
    UserDao* dao = UserDao::getInstance();
    int id = dao->insertUser(ue);//插入数据库
    //返回id信息
    QString retData = MsgBuilder::buildRegisterUserReturnMsg(id);
    socket->write(retData.toLocal8Bit());
}

void NetWorkThread::dealLogin(QString jsonStr)
{
    //解析数据
    MsgBuilder::UserData data = MsgBuilder::parseLoginMsg(jsonStr);
    //查询数据库
    UserEntity ue;
    ue.password = data.password;
    ue.id = data.id;
    UserDao* dao = UserDao::getInstance();
    if(dao->isPasswordRight(ue))//登录成功
    {
        data.nickname = ue.nickname;
        data.headId = ue.headId;
        user = data;//线程持有用户信息
        emit loginSuccess(data, this);
    }
    else//登录失败
    {
        QString ret = MsgBuilder::buildLoginLoseReturnMsg();
        socket->write(ret.toLocal8Bit());
    }
}

void NetWorkThread::dealLoginSuc(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::dealSendMsg(QString jsonStr)
{
    MsgBuilder::UserData from;
    MsgBuilder::UserData to;
    QString msg = MsgBuilder::parseSendMsg(jsonStr, from, to);
    emit sendMsg(from, to, msg);
}

void NetWorkThread::dealReceiveMsg(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::dealUserOnline(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::dealUserOffline(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::dealCreateGroup(QString jsonStr)
{
    QString name = MsgBuilder::parseCreateGroup(jsonStr);
    emit createGroup(name, this);
}

void NetWorkThread::dealCreateGroupReturn(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::dealSearchGroup(QString jsonStr)
{
    emit searchGroup(this);
}

void NetWorkThread::dealSearchGroupReturn(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::dealJoinGroup(QString jsonStr)
{
    int groupId;
    MsgBuilder::UserData user = MsgBuilder::parseJoinGroup(jsonStr, groupId);
    emit joinGroup(this, groupId);
}

void NetWorkThread::dealJoinGroupReturn(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::dealUserJoinGroup(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::dealLeaveGroup(QString jsonStr)
{
    MsgBuilder::UserData user;
    int groupId = MsgBuilder::parseLeaveGroup(jsonStr, user);
    emit leaveGroup(this, groupId);
}

void NetWorkThread::dealUserLeaveGroup(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::dealSendGroupMsg(QString jsonStr)
{
    int groupId;
    MsgBuilder::UserData user;
    QString msg = MsgBuilder::parseSendGroupMsg(jsonStr, user, groupId);
    emit sendGroupMsg(this, user, groupId, msg);
}

void NetWorkThread::dealReceiveGroupMsg(QString jsonStr)
{
    socket->write(jsonStr.toLocal8Bit());
}

void NetWorkThread::readyRead()
{
    isRead = true;
}

void NetWorkThread::disconnected()
{
    isDisconnected = true;
}

