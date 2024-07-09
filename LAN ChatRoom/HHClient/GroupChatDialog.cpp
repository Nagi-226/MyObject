#include "GroupChatDialog.h"
#include "ui_groupchatdialog.h"
#include <QGridLayout>
#include <QBoxLayout>
#include "ChatItem.h"

GroupChatDialog::GroupChatDialog(int groupId, QString name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupChatDialog),
    groupId(groupId),
    name(name)
{
    ui->setupUi(this);
    initUI();
    socket = HHTcpSocket::getInstance();
}

GroupChatDialog::~GroupChatDialog()
{
    delete ui;
}

void GroupChatDialog::addGroupFriends(vector<MsgBuilder::UserData> &groupFriends)
{
    for(int i = 0;i < groupFriends.size();i++)
    {
        addGroupFriend(groupFriends[i]);
    }
}

void GroupChatDialog::addGroupFriend(MsgBuilder::UserData groupFriend)
{
    FriendItem* friendItem = new FriendItem(groupFriend, ui->listWidget_friends);   
    QListWidgetItem* listWidgetItem = new QListWidgetItem(ui->listWidget_friends);
    listWidgetItem->setSizeHint(friendItem->size());
    ui->listWidget_friends->setItemWidget(listWidgetItem, friendItem);
}

void GroupChatDialog::removeGroupFriend(MsgBuilder::UserData groupFriend)
{
    for(int i = 0;i < ui->listWidget_friends->count();i++)
    {
        QListWidgetItem* listWidgetItem = ui->listWidget_friends->item(i);
        FriendItem* fItem = dynamic_cast<FriendItem*>(ui->listWidget_friends->itemWidget(listWidgetItem));
        MsgBuilder::UserData user = fItem->getUserData();
        if(user.id == groupFriend.id)
        {
            ui->listWidget_friends->removeItemWidget(listWidgetItem);
            delete listWidgetItem;
        }
    }
}

void GroupChatDialog::addReceiveMsg(MsgBuilder::UserData user, QString msg)
{
    chatWidget->addMsg(msg, user.headId, ChatItem::guest);
}

void GroupChatDialog::initUI()
{
    QGridLayout* gridLayout = new QGridLayout(this);
    setLayout(gridLayout);
    chatWidget = new ChatWidget(this);

    gridLayout->addWidget(chatWidget, 0, 0, 1, 1);
    gridLayout->addWidget(ui->listWidget_friends, 0, 1, 3, 1);
    
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(ui->pushButton_brow);
    horizontalLayout->addWidget(ui->pushButton_3);
    horizontalLayout->addWidget(ui->pushButton_send);
    gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);
    gridLayout->addWidget(ui->textEdit, 2, 0, 1, 1);
    
    setWindowTitle(QString("%1(%2)").arg(name).arg(groupId));
}

void GroupChatDialog::on_pushButton_send_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    if(msg.isEmpty())
        return;
    QString data = MsgBuilder::buildSendGroupMsg(socket->getHostData(), groupId, msg);
    socket->write(data.toLocal8Bit());
    
    chatWidget->addMsg(msg, socket->getHostData().headId, ChatItem::host);
}
