#ifndef ONEFRIEND_H
#define ONEFRIEND_H

#include <QWidget>
#include"msgmark.h"
#include"msgbuilder.h"

namespace Ui {
class OneFriend;
}

class OneFriend : public QWidget
{
    Q_OBJECT

public:
    explicit OneFriend(UserData,QWidget *parent = 0);
    ~OneFriend();

   inline UserData getUserData(){return udata;}
   void addWaitMsg(QString msg);
   QStringList getMsgs();

   //未读消息提示
   void msgAlarm(bool);

private:
    Ui::OneFriend *ui;
    UserData udata;

    QStringList waitMsg;//存储未读消息
    msgMark *m;//显示未读消息数量
};

#endif // ONEFRIEND_H
