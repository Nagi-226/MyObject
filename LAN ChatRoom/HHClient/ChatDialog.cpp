#include "ChatDialog.h"
#include "ui_chatdialog.h"
#include <QGridLayout>
#include <QHBoxLayout>

ChatDialog::ChatDialog(QStringList waitMsgs, QWidget* friendItem, MsgBuilder::UserData user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog),
    user(user),
    friendItem(friendItem)
{
    ui->setupUi(this);
    initSocket();
    initUI(waitMsgs);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::socketData(QString data)
{
    
}

void ChatDialog::addReceiveMsg(MsgBuilder::UserData guest, QString msg)
{
    qDebug()<<"addReceiveMsg  "<<guest.headId;
    chatWidget->addMsg(msg, user.headId, ChatItem::guest);
}

void ChatDialog::initSocket()
{
    socket = HHTcpSocket::getInstance();
}

void ChatDialog::initUI(QStringList waitMsgs)
{
    QGridLayout* gridLayout = new QGridLayout(this);//总布局
    setLayout(gridLayout);
    
    chatWidget = new ChatWidget(this);//显示聊天信息控件（自定义）
    gridLayout->addWidget(chatWidget, 0, 0, 1, 2);
       
    gridLayout->addWidget(ui->textEdit, 2, 0, 1, 1);
    gridLayout->addWidget(ui->pushButton_send, 2, 1, 1, 1);
    
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(ui->pushButton_brow);
    horizontalLayout->addWidget(ui->pushButton_2);
    gridLayout->addLayout(horizontalLayout, 1, 0, 1, 2);
    
    setWindowTitle(QString("%1(%2)").arg(user.nickname).arg(user.id));
    
    for(QStringList::iterator iter = waitMsgs.begin();iter != waitMsgs.end();iter++)
    {
        chatWidget->addMsg(*iter, user.headId, ChatItem::guest);
    }
}

void ChatDialog::on_pushButton_send_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    if(msg.isEmpty())
        return;
    QString sendMsg = MsgBuilder::buildSendMsg(socket->getHostData(), user, msg);
    socket->write(sendMsg.toLocal8Bit());
    chatWidget->addMsg(msg, HHTcpSocket::getInstance()->getHostData().headId, ChatItem::host);
}
