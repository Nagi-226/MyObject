#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QWidget>
#include"onefriend.h"
#include"usersocket.h"
#include"msgbuilder.h"
#include"chatdialog.h"
#include<QListWidgetItem>//列表项类
#include<QStringList>//字符串集合类
namespace Ui {
class FriendList;
}

class FriendList : public QWidget
{
    Q_OBJECT

public:
    explicit FriendList(QString,QWidget *parent = 0);
    ~FriendList();

    void on_readyRead_slot();
    void on_disconnected_slot();

    void initFriendList(QString);
    void dealUserOnline(QString);
   void dealUserOffLine(QString);
   void dealUserReceiveMsg(QString);

private slots:
   void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

   void on_sigUserDialogClose_slot(UserData);
private:
    Ui::FriendList *ui;
    userSocket *socket;
    //管理已经打开的用户私聊窗口 如果窗口已经打开 则存入容器
    map<UserData,ChatDialog*> usersDialog;

};

#endif // FRIENDLIST_H
