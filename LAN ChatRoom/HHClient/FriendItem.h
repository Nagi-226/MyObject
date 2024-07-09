#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>
#include "MsgBuilder.h"
#include "HHTcpSocket.h"
#include "MsgMarke.h"

namespace Ui {
class FriendItem;
}

class FriendItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit FriendItem(MsgBuilder::UserData user, QWidget *parent = 0);
    ~FriendItem();
    
    inline MsgBuilder::UserData getUserData(){return user;}
    QStringList getWaitMsgs();
    inline void addWaitMsg(QString msg){waitMsgs.append(msg);mm->setNum(waitMsgs.size());}
    
    void msgAlarm(bool open);//消息提醒
private:
    Ui::FriendItem *ui;
    MsgBuilder::UserData user;
    QStringList waitMsgs;//打开窗口前保存的聊天信息
    MsgMarke* mm;//消息提示小红点
};

#endif // FRIENDITEM_H
