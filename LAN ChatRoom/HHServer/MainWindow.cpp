#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QHostInfo>
#include "NetworkThread.h"
#include <QList>
#include <QDebug>
#include <algorithm>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qRegisterMetaType<MsgBuilder::UserData>("MsgBuilder::UserData");
    ui->setupUi(this);
    showHostIp();
    tcpServer.listen(QHostAddress::Any, PORT);
    connect(&tcpServer, SIGNAL(newConnectionForThread(qintptr)), this, SLOT(newConnection(qintptr)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showHostIp()
{
    QString localHostName = QHostInfo::localHostName();      
    QHostInfo info = QHostInfo::fromName(localHostName);
    
    QList<QHostAddress> list = info.addresses();
    QString ip;
    for(int i = 0;i < list.size();i++)
    {
        if(list[i].protocol() == QAbstractSocket::IPv4Protocol)
        {
            ip+="*";
            ip+=list[i].toString();  
        }
    }
    ui->label_ip->setText(ip);
}

void MainWindow::newConnection(qintptr handle)
{
    NetWorkThread* thread = new NetWorkThread(handle, this);
    connect(thread, SIGNAL(loginSuccess(MsgBuilder::UserData,QThread*)), this, SLOT(loginSuccess(MsgBuilder::UserData,QThread*)));
    connect(thread, SIGNAL(sendMsg(MsgBuilder::UserData,MsgBuilder::UserData,QString)), this, SLOT(sendMsg(MsgBuilder::UserData,MsgBuilder::UserData,QString)));
    connect(thread, SIGNAL(userOffline(MsgBuilder::UserData)), this, SLOT(userOffline(MsgBuilder::UserData)));
    connect(thread, SIGNAL(createGroup(QString, QThread*)), this, SLOT(createGroup(QString, QThread*)));
    connect(thread, SIGNAL(searchGroup(QThread*)), this, SLOT(searchGroup(QThread*)));
    connect(thread, SIGNAL(joinGroup(QThread*,int)), this, SLOT(joinGroup(QThread*,int)));
    connect(thread, SIGNAL(leaveGroup(QThread*,int)), this, SLOT(leaveGroup(QThread*,int)));
    connect(thread, SIGNAL(sendGroupMsg(QThread*,MsgBuilder::UserData,int,QString)), this, SLOT(sendGroupMsg(QThread*,MsgBuilder::UserData,int,QString)));
    thread->start();
}

void MainWindow::loginSuccess(MsgBuilder::UserData user, QThread *thread)
{ 
    if(usersOnline.count(user))//已经登录成功
    {
        return;
    }
    
    
    //遍历所有在线人
    vector<MsgBuilder::UserData> data;
    for(map<MsgBuilder::UserData,QThread*>::iterator iter = usersOnline.begin();
        iter != usersOnline.end();iter++)
    {
        data.push_back(iter->first);//整理在线人数据准备发送给登录人
        NetWorkThread* userThread = dynamic_cast<NetWorkThread*>(iter->second);
        userThread->addUserOnlineInfo(user);//通知所有人用户上线
    }
    //返回在线列表信息给登录人
    NetWorkThread* netWorkThread = dynamic_cast<NetWorkThread*>(thread);
    netWorkThread->addLoginSuccessInfo(user, data);
    //将这个人家到在线列表中
    usersOnline.insert(pair<MsgBuilder::UserData,QThread*>(user, thread));
}

void MainWindow::sendMsg(MsgBuilder::UserData from, MsgBuilder::UserData to, QString msg)
{
    if(usersOnline.count(to) == 0)
    {
        return;
    }
    //找到接受用户线程
    NetWorkThread* netWorkThread = dynamic_cast<NetWorkThread*>(usersOnline[to]);
    netWorkThread->addReceiveMsgInfo(from, to, msg);
    QString info = QString("from %1 to %2 : %3").arg(from.id).arg(to.id).arg(msg);
    ui->textBrowser->append(info);
}

void MainWindow::userOffline(MsgBuilder::UserData user)
{
    if(usersOnline.count(user) == 1)
    {
        usersOnline.erase(user);
    }
    //遍历所有在线人
    vector<MsgBuilder::UserData> data;
    for(map<MsgBuilder::UserData,QThread*>::iterator iter = usersOnline.begin();
        iter != usersOnline.end();iter++)
    {
        data.push_back(iter->first);//整理在线人数据准备发送给登录人
        NetWorkThread* userThread = dynamic_cast<NetWorkThread*>(iter->second);
        userThread->addUserOfflineInfo(user);//通知所有人用户下线
    }
}

void MainWindow::createGroup(QString name, QThread* thread)
{
    GroupKey gk;
    gk.groupId = createGroupId++;
    gk.name = name;
    groups[gk] = vector<QThread*>();
    NetWorkThread* netWorkThread = dynamic_cast<NetWorkThread*>(thread);
    netWorkThread->addCreateGroupReturnInfo(gk.groupId);
}

void MainWindow::searchGroup(QThread *thread)
{
    vector<int> ids;
    vector<QString> names;
    for(map<GroupKey, vector<QThread*> >::iterator iter = groups.begin();iter != groups.end();iter++)
    {
        ids.push_back(iter->first.groupId);
        names.push_back(iter->first.name);
    }
    NetWorkThread* netWorkThread = dynamic_cast<NetWorkThread*>(thread);
    netWorkThread->addSearchGroupReturnInfo(ids, names);
}

void MainWindow::joinGroup(QThread *thread, int groupId)
{   
    NetWorkThread* netWorkThread = dynamic_cast<NetWorkThread*>(thread);
    GroupKey gk;
    gk.groupId = groupId;
    groups[gk].push_back(thread);//用户加入群聊
    //返回用户群聊中的用户
    vector<MsgBuilder::UserData> groupFriends;
    vector<QThread*> threads = groups[gk];
    for(int i = 0;i < threads.size();i++)
    {
        NetWorkThread* groupFriend = dynamic_cast<NetWorkThread*>(threads[i]);
        groupFriends.push_back(groupFriend->getUserData());
        if(groupFriend != netWorkThread)//通知群内其他用户上线信息
        {
            groupFriend->addUserJoinGroupInfo(netWorkThread->getUserData(), groupId);
        }
    }
    netWorkThread->addJoinGroupReturnInfo(groupId, groupFriends);
}

void MainWindow::leaveGroup(QThread *thread, int groupId)
{
    NetWorkThread* netWorkThread = dynamic_cast<NetWorkThread*>(thread);
    GroupKey gk;
    gk.groupId = groupId;
    for(vector<QThread*>::iterator iter = groups[gk].begin();iter != groups[gk].end();)
    {
        if(*iter == thread)
        {
            iter = groups[gk].erase(iter);
        }
        else
        {
            NetWorkThread* groupFriend = dynamic_cast<NetWorkThread*>(*iter);
            groupFriend->addUserLeaveGroupInfo(netWorkThread->getUserData(), groupId);
            iter++;
        }
    }
}

void MainWindow::sendGroupMsg(QThread *thread, MsgBuilder::UserData from, int to, QString msg)
{
    GroupKey gk;
    gk.groupId = to;
    vector<QThread*> threads = groups[gk];
    for(int i = 0;i < threads.size();i++)
    {
        if(thread == threads[i])//跳过自己
        {
            continue;
        }
        NetWorkThread* groupFriend = dynamic_cast<NetWorkThread*>(threads[i]);
        groupFriend->addReceiveGroupMsgInfo(from, to, msg);
    }
}
