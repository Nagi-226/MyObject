#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "HHTcpSocket.h"
#include "MsgBuilder.h"
#include "ChatWidget.h"

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT
    
public:
    ChatDialog(QStringList waitMsgs, QWidget* friendItem, MsgBuilder::UserData user, QWidget *parent = 0);
    ~ChatDialog();
    
    inline MsgBuilder::UserData getuserData(){return user;}
    inline QWidget* getFriendItem(){return friendItem;}
    
    void socketData(QString data);
    void addReceiveMsg(MsgBuilder::UserData guest, QString msg);
private slots:
    void on_pushButton_send_clicked();
    
private:
    void initSocket();
    void initUI(QStringList waitMsgs);
private:
    Ui::ChatDialog *ui;
    HHTcpSocket* socket;
    ChatWidget* chatWidget;
    MsgBuilder::UserData user;//聊天的用户
    QWidget* friendItem;
};

#endif // CHATDIALOG_H
