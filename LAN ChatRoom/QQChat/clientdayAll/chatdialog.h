#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include<QStringList>//字符串集合类
#include"msgbuilder.h"
#include<QDateTime>//时间日期类
#include<QMessageBox>
#include"usersocket.h"
#include<QCloseEvent>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QStringList,UserData, QWidget *parent = 0);
    ~ChatDialog();
    void getWaitMsg(QStringList);
    void addUserChatMsg(QString);

protected:
    //重写关闭事件
    void closeEvent(QCloseEvent *);
private slots:
    void on_pushButton_snd_clicked();

private:
    Ui::ChatDialog *ui;
    UserData udata;//记录当前用户信息
    userSocket *socket;
signals:
    void sigUserDialogClose(UserData);
};

#endif // CHATDIALOG_H
