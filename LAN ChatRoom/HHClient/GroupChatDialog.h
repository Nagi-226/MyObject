#ifndef GROUPCHATDIALOG_H
#define GROUPCHATDIALOG_H

#include <QDialog>
#include <vector>
#include "MsgBuilder.h"
#include "FriendItem.h"
#include <QListWidgetItem>
#include "HHTcpSocket.h"
#include "ChatWidget.h"

using namespace std;

namespace Ui {
class GroupChatDialog;
}

class GroupChatDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GroupChatDialog(int groupId, QString name, QWidget *parent = 0);
    ~GroupChatDialog();
    
    inline int getGroupId(){return groupId;}
    void addGroupFriends(vector<MsgBuilder::UserData>& groupFriends);
    void addGroupFriend(MsgBuilder::UserData groupFriend);
    void removeGroupFriend(MsgBuilder::UserData groupFriend);
    void addReceiveMsg(MsgBuilder::UserData user, QString msg);
private slots:
    void on_pushButton_send_clicked();
    
private:
    void initUI();
    
private:
    Ui::GroupChatDialog *ui;
    int groupId;
    QString name;
    HHTcpSocket* socket;
    ChatWidget* chatWidget;
};

#endif // GROUPCHATDIALOG_H
