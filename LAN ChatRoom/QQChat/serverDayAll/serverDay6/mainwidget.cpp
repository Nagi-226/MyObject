#include "mainwidget.h"

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
{
    qRegisterMetaType<UserData>("UserData");
    //实例化server对象
    mtServer = new mulThreadServer;

    //监听
    mtServer->listen(QHostAddress::Any,6666);

    //有新连接时 会收到这个信号
    connect(mtServer,&mulThreadServer::newConnection_signal,this,&mainWidget::deal_newconnection_slot);
}

mainWidget::~mainWidget()
{

}

//新用户登录成功 调用
void mainWidget::on_loginSucSignal_slot(UserData udata,SoketThread *thread)//参数1：新登录的用户信息 参数2：新登录用户对应的子线程
{

    //查重
    if(users.count(udata)==1)//count map容器函数
    {
        //已经登录
        return ;
    }

    vector<UserData> v;//存储所有已经在线的用户信息

    //1.遍历map容器
    for(map<UserData,SoketThread*>::iterator iter=users.begin();iter!=users.end();iter++)
    {
       // iter->first//key 用户信息
        //iter->second//value 子线程

        //所有用户信息存入vector容器
        v.push_back(iter->first);

        //将新登录用户信息通知所有在线客户端
        iter->second->addUserOnLine(udata);
    }

    //2.将所有在线用户信息 传递给新登录用户  新登录的+已经在线
    //
     thread->addUserLoginSuc(v,udata);


    //3.将新登录用户添加到map容器
    users.insert(pair<UserData,SoketThread*>(udata,thread));


}


//有连接会调用
void mainWidget::deal_newconnection_slot(qintptr descriptor)
{

    qDebug()<<"有人来啦.....";

    //创建子线程
    std = new SoketThread(descriptor);

    //信号槽连接
    connect(std,&SoketThread::finished,std,&SoketThread::deleteLater);

    //子线程发来信号 通知新用户登录成功
    connect(std,&SoketThread::loginSucSignal,this,&mainWidget::on_loginSucSignal_slot);

    //子线程发来信号 通知有好友下线
    connect(std,&SoketThread::sigUserOffLine,this,&mainWidget::on_sigUserOffLine);


    //启动线程
    std->start();
}

//参数 下线好友的用户信息
void mainWidget::on_sigUserOffLine(UserData udata)
{
    //遍历容器 通知所有在线好友 客户下线
    for(map<UserData,SoketThread*>::iterator iter=users.begin();iter!=users.end();iter++)
    {
        iter->second->addUserOffLine(udata);
    }

}








