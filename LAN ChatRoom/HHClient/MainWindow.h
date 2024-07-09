#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "HHTcpSocket.h"
#include "MsgBuilder.h"
#include <vector>
#include <QListWidgetItem>
#include "FriendItem.h"
#include "ChatDialog.h"
#include <map>
#include "CreateGroupDialog.h"
#include "SearchGroupDialog.h"
#include "GroupChatDialog.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(vector<MsgBuilder::UserData> friends, QWidget *parent = 0);
    ~MainWindow();
    struct ItemAndDialog
    {
        FriendItem* item;
        ChatDialog* dialog;
    };
    void addFriendsList(MsgBuilder::UserData user);//添加好友信息
    
private slots:
    void readyRead();
    void on_listWidget_friends_itemDoubleClicked(QListWidgetItem *item);
    void rejected();
    void createGroupDialogRejected();
    void searchGroupDialogRejected();
    void groupChatDialogRejected();
    void createGroupChatDialog(int id, QString name);
    void on_pushButton_createGroup_clicked();
    
    void on_pushButton_searchGroup_clicked();
    
private:
    void initSocket();
    void initFriendsList(vector<MsgBuilder::UserData> friends);//初始化好友信息   
    void receiveMsg(QString jsonStr);
    void removeFromFriendList(MsgBuilder::UserData user);//删除好友信息
    void createGroupReturn(QString jsonStr);
    void searchGroupReturn(QString jsonStr);
    void joinGroupReturn(QString jsonStr);
    void userJoinGroup(QString jsonStr);
    void userLeaveGroup(QString jsonStr);
    void receiveGroupMsg(QString jsonStr);
private:
    Ui::MainWindow *ui;
    HHTcpSocket* socket;
    map<MsgBuilder::UserData, ItemAndDialog> chatFriends;//正在聊天的好友
    CreateGroupDialog* createGroupDialog = 0;
    SearchGroupDialog* searchGroupDialog = 0;
    map<int, GroupChatDialog*> groupChats;//群聊窗口
};

#endif // MAINWINDOW_H
