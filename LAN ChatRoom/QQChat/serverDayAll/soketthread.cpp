#include "soketthread.h"

SoketThread::SoketThread(qintptr descriptor,QObject *parent) : QThread(parent),sd(descriptor),isRun(true),isRead(false)
{
    //初始状态：连接未断开 套接字里没有数据可读
}

SoketThread::~SoketThread()
{

}

void SoketThread::addUserLoginSuc(vector<UserData> other, UserData self)
{
    //将数据构建  自己用户信息+其他在线好友信息
    QString jsonStr =  MsgBuilder::buildUserLoginSucMsg(self,other);
    qDebug()<<"add"<<jsonStr;
    //存入队列 子线程取数据处理
    mutex.lock();
    msgQueue.push(jsonStr);
    mutex.unlock();
}

void SoketThread::addUserOnLine(UserData udata)
{
    //构建
   QString jsonStr = MsgBuilder::buildUserOnlineMsg(udata);

   //存入队列 子线程取数据处理
   mutex.lock();
   msgQueue.push(jsonStr);
   mutex.unlock();
}

void SoketThread::addUserOffLine(UserData udata)
{
    //构建
    QString jsonStr = MsgBuilder::buildUserOfflineMsg(udata);

    //存入队列 子线程取数据处理
    mutex.lock();
    msgQueue.push(jsonStr);
    mutex.unlock();
}

void SoketThread::addUserSendMsg(QString jsonStr)
{
   //存入队列 子线程取数据处理
   mutex.lock();
   msgQueue.push(jsonStr);
   mutex.unlock();


}




void SoketThread::run()
{

    qDebug()<<"run....";

    //设置套接字
    socket = new QTcpSocket;
    socket->setSocketDescriptor(sd);

    //信号槽函数连接

    //有消息到来
    connect(socket,&QTcpSocket::readyRead,this,&SoketThread::on_readyRead_slot);

    //连接断开
    connect(socket,&QTcpSocket::disconnected,this,&SoketThread::on_disconnected_slot);


    //通信
    QByteArray msg;
    QString str,strMsg;
    while(isRun)//连接没有断开
    {
        if(isRead)//有消息到来
        {
            //从套接字读消息
            msg =  socket->readAll();
            str = QString::fromLocal8Bit(msg);

            //存入消息队列
            //上锁 保护临界资源
            mutex.lock();
            msgQueue.push(str);
            //解锁
            mutex.unlock();
            isRead = false;
        }


        //循环出队 队列数据=套接字数据+主线程处理
        while(!msgQueue.empty())//队列不为空
        {
            //获取队首元素
            strMsg = msgQueue.front();
            //出队删除

            //上锁
            mutex.lock();
            msgQueue.pop();
            //解锁
            mutex.unlock();


            //处理消息   getType :根据json串获取类型
            switch(MsgBuilder::getType(strMsg))
            {


            case  MsgBuilder::userRegister:
                dealUserRegister(strMsg);//处理客户端注册请求
                break;
            case MsgBuilder::userLogin:
                dealUserLogin(strMsg);
                break;
            case MsgBuilder::userLoginSuc://登录成功
                dealUserLoginSuc(strMsg);//发给客户端
                break;
            case MsgBuilder::userOnline://上线
                dealUserOnLine(strMsg);
                 break;
            case MsgBuilder::userOffline://下线
                dealUserOffLine(strMsg);
                 break;
            case MsgBuilder::userSendMsg://私聊发送消息
                dealUserSend(strMsg);
                break;

            case MsgBuilder::userReceiveMsg://有新消息到来
                dealUserReceiveMsg(strMsg);
                break;

            }
        }
        //延时(毫秒为单位)
        socket->waitForReadyRead(1000);
    }

//通知主线程 客户端下线
    emit sigUserOffLine(udata);

//关闭套接字
socket->close();
//删除套接字
delete socket;

}

void SoketThread::dealUserSend(QString jsonStr)
{
    emit sigUserSendMsg(jsonStr);
}
void SoketThread::dealUserOffLine(QString jsonStr)
{
    //发送本线程服务的客户端
    QByteArray bdata= jsonStr.toLocal8Bit();
    socket->write(bdata.data(),bdata.size());
}



//收到新消息 发给服务的客户端
void SoketThread::dealUserReceiveMsg(QString jsonStr)
{
    //发送本线程服务的客户端
    QByteArray bdata= jsonStr.toLocal8Bit();
    socket->write(bdata.data(),bdata.size());
}

void SoketThread::dealUserOnLine(QString jsonStr)
{
    //发送本线程服务的客户端
    QByteArray bdata= jsonStr.toLocal8Bit();
    socket->write(bdata.data(),bdata.size());

}


void SoketThread::dealUserLoginSuc(QString jsonStr)
{
    qDebug()<<"dealsuc"<<jsonStr;
    //将数据(类型 登录成功)发送给客户端
    QByteArray bdata =  jsonStr.toLocal8Bit();
    socket->write(bdata.data(),bdata.size());
}



void SoketThread::dealUserRegister(QString jsonStr)
{
    //解析json串
    //将json串转为UserData形式 由userData带回
    UserData usrdata;
    MsgBuilder::parseUserRegisterMsg(jsonStr, usrdata);


    //存入数据库
    //1.取得数据库对象
    UserDao *p = UserDao::getInstance();

    //2.插入数据
    UserEntity e;

    e.nickName = usrdata.nickName;
    e.passwd = usrdata.passwd;
    e.headId = usrdata.headId;
    p->insertUser(e);

    //返回id信息
    usrdata.userId = e.userId;
    QString retData = MsgBuilder::buildUserRegisterReturnMsg(usrdata);

    QByteArray bdata =retData.toLocal8Bit();

    //发送给客户端
    socket->write(bdata.data(),bdata.size());


}

void SoketThread::dealUserLogin(QString jsonStr)
{
    UserData udata;
    MsgBuilder::parseUserLoginMsg(jsonStr,udata);

    //1.数据库匹配
    bool ok;//记录登录是否成功
    UserEntity e;
    e.userId = udata.userId;
    e.passwd = udata.passwd;

    UserDao::getInstance()->selectUser(e,ok);

    //udata中 只有userId passwd
    udata.nickName = e.nickName;
    udata.headId=e.headId;

    //判断用户名密码是否正确
    if(ok)
    {
        qDebug()<<"deal login:"<<jsonStr;

        //通知主线程--管理所有客户端信息
        emit loginSucSignal(udata,this);//发送信号 携带用户信息

        //记录当前登录用户信息
        this->udata = udata;
    }
    else
    {
        //登录失败
        //回发客户端

        //构建 登录失败
        QString jsonStr = MsgBuilder::buildUserLoginLoseMsg();
        QByteArray bdata = jsonStr.toLocal8Bit();

        //发送客户端
        socket->write(bdata.data(),bdata.size());

    }
}





void SoketThread::on_readyRead_slot()
{
    isRead = true;//套接字有数据可读
}

void SoketThread::on_disconnected_slot()
{
    isRun = false;//标记连接已经断开
}


