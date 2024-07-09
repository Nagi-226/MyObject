#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <queue>
#include <vector>
#include "MsgBuilder.h"
#include <QMutex>

using namespace std;

class NetWorkThread : public QThread
{
    Q_OBJECT
public:
    NetWorkThread();
    NetWorkThread(qintptr handle, QObject* parent);
    ~NetWorkThread();

    inline MsgBuilder::UserData getUserData(){return user;}
    
    //把在线好友信息加入消息队列等待线程发送
    void addLoginSuccessInfo(MsgBuilder::UserData user, vector<MsgBuilder::UserData>& friends);
    //用户接受消息
    void addReceiveMsgInfo(MsgBuilder::UserData from, MsgBuilder::UserData to, QString msg);
    //用户上线消息
    void addUserOnlineInfo(MsgBuilder::UserData user);
    //用户下线消息
    void addUserOfflineInfo(MsgBuilder::UserData user);
    //创建群返回
    void addCreateGroupReturnInfo(int groupId);
    //查找群信息返回
    void addSearchGroupReturnInfo(vector<int>& ids, vector<QString>& names);
    //加入群聊返回
    void addJoinGroupReturnInfo(int groupId, vector<MsgBuilder::UserData>& groupFriends);
    //通知新用户加入群聊
    void addUserJoinGroupInfo(MsgBuilder::UserData user, int groupId);
    //通知用户离开群聊
    void addUserLeaveGroupInfo(MsgBuilder::UserData user, int groupId);
    //接收群聊消息
    void addReceiveGroupMsgInfo(MsgBuilder::UserData from, int to, QString msg);
private:
    void run();//重新run函数，是线程要执行的逻辑
    
    void dealRegisterUser(QString jsonStr);
    void dealLogin(QString jsonStr);
    void dealLoginSuc(QString jsonStr);
    void dealSendMsg(QString jsonStr);
    void dealReceiveMsg(QString jsonStr);
    void dealUserOnline(QString jsonStr);
    void dealUserOffline(QString jsonStr);
    void dealCreateGroup(QString jsonStr);
    void dealCreateGroupReturn(QString jsonStr);
    void dealSearchGroup(QString jsonStr);
    void dealSearchGroupReturn(QString jsonStr);
    void dealJoinGroup(QString jsonStr);
    void dealJoinGroupReturn(QString jsonStr);
    void dealUserJoinGroup(QString jsonStr);
    void dealLeaveGroup(QString jsonStr);
    void dealUserLeaveGroup(QString jsonStr);
    void dealSendGroupMsg(QString jsonStr);
    void dealReceiveGroupMsg(QString jsonStr);
private slots:
    void readyRead();
    void disconnected();
signals://这些信号都是用来与主线程进行线程间通信的，信号是线程安全的
    void loginSuccess(MsgBuilder::UserData user, QThread* thread);
    void sendMsg(MsgBuilder::UserData from, MsgBuilder::UserData to, QString msg);
    void userOffline(MsgBuilder::UserData user);
    void createGroup(QString name, QThread* self);
    void searchGroup(QThread* self);
    void joinGroup(QThread* self, int groupId);
    void leaveGroup(QThread* self, int groupId);
    void sendGroupMsg(QThread* self, MsgBuilder::UserData from, int to, QString msg);
private:
    QTcpSocket* socket;
    queue<QString> msgQueue;//存放接收到数据，都是格式化好的json字符串
    bool isDisconnected = false;
    bool isRead = false;
    int test;
    QMutex mutex;
    MsgBuilder::UserData user;
};

#endif // NETWORKTHREAD_H
