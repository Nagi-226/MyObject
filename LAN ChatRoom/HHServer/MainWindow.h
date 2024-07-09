#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TcpServerForThread.h"
#include "Publicdef.h"
#include <map>
#include <vector>
#include "MsgBuilder.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    
    struct GroupKey
    {
        int groupId;
        QString name;
        bool operator<(const GroupKey& other) const
        {
            return groupId < other.groupId;
        }
    };
    
    void showHostIp();
private slots:
    void newConnection(qintptr handle);
    void loginSuccess(MsgBuilder::UserData user,QThread* thread);
    void sendMsg(MsgBuilder::UserData from, MsgBuilder::UserData to, QString msg);
    void userOffline(MsgBuilder::UserData user);
    void createGroup(QString name, QThread* thread);
    void searchGroup(QThread* thread);
    void joinGroup(QThread* thread, int groupId);
    void leaveGroup(QThread*thread ,int groupId);
    void sendGroupMsg(QThread* thread, MsgBuilder::UserData from, int to, QString msg);
private:
    Ui::MainWindow *ui;
    TcpServerForThread tcpServer;
    map<MsgBuilder::UserData, QThread*> usersOnline;//在线用户
    int createGroupId = 200000;//创建群聊用的ID
    map<GroupKey, vector<QThread*> > groups;//群聊，每个键值对是一个群聊
};

#endif // MAINWINDOW_H
