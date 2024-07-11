#include "chatdialog.h"
#include "ui_chatdialog.h"

ChatDialog::ChatDialog(QStringList waitMsg,UserData udata,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog),udata(udata)
{
    ui->setupUi(this);
    socket = userSocket::getInstance();

    //窗口标题设置
    this->setWindowTitle(udata.nickName);

    getWaitMsg(waitMsg);

}

ChatDialog::~ChatDialog()
{
    delete ui;
}

//获取未读消息
void ChatDialog::getWaitMsg(QStringList msg)
{
    for(QStringList::iterator iter=msg.begin();iter!=msg.end();iter++)
    {
        //将容器内QString字符串 添加到显示框
        addUserChatMsg(*iter);
    }
}

//将形参msg 数据显示到私聊窗口的显示框
void ChatDialog::addUserChatMsg(QString msg)
{
    //获取当前时间 并转为字符串格式  yyyy-MM-dd hh:mm:ss是时间显示的格式
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");


    //设置文本颜色 蓝色
    ui->textBrowser->setTextColor(Qt::blue);

    //追加消息并显示
    ui->textBrowser->append("["+udata.nickName+"]"+time);

    //setCurrentFont 设置字体
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));

    //追加显示
    ui->textBrowser->append(msg);
}

void ChatDialog::closeEvent(QCloseEvent *)
{
    //通知好友列表 管理私聊窗口
    emit sigUserDialogClose(udata);

}

void ChatDialog::on_pushButton_snd_clicked()
{
    //如果没有输入文本 错误提示
    if(ui->textEdit->toPlainText()=="")
    {
        QMessageBox::information(this,"文本内容有误","文本不能为空");
        return;
    }

    //提取文本框内容  富文本格式
    QString msg = ui->textEdit->toHtml();

    QString jsonStr = MsgBuilder::buildUserSendMsg(socket->getData(),udata,msg);

    QByteArray bdata = jsonStr.toLocal8Bit();
    socket->write(bdata.data(),bdata.size());




    //本聊天窗口的显示框也要显示发送的内容
    //获取当前时间 并转为字符串格式  yyyy-MM-dd hh:mm:ss是时间显示的格式
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");


    //设置文本颜色 蓝色
    ui->textBrowser->setTextColor(Qt::red);

    //追加消息并显示 socket->getData()得到当前登录用户信息
    ui->textBrowser->append("["+socket->getData().nickName+"]"+time);

    //setCurrentFont 设置字体
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));

    //追加显示
    ui->textBrowser->append(msg);

    ui->textEdit->clear();//清空



}
























