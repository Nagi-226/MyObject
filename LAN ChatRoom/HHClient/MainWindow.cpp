
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QDebug>


MainWindow::MainWindow(vector<MsgBuilder::UserData> friends, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initSocket();
    initFriendsList(friends);
    MsgBuilder::UserData hostData = socket->getHostData();
    ui->label_hostname->setText(QString("%1(%2)").arg(hostData.nickname).arg(hostData.id));
}

MainWindow::~MainWindow()
{
    delete ui;
    HHTcpSocket::releaseInstance();
}

void MainWindow::addFriendsList(MsgBuilder::UserData user)
{
    qDebug()<<"addFriendsList  "<<user.headId;
    FriendItem* fItem = new FriendItem(user, ui->listWidget_friends);
    QListWidgetItem* lItem = new QListWidgetItem(ui->listWidget_friends);
    lItem->setSizeHint(fItem->size());
    ui->listWidget_friends->setItemWidget(lItem, fItem);
}

void MainWindow::removeFromFriendList(MsgBuilder::UserData user)
{
    for(int i = 0;i < ui->listWidget_friends->count();i++)
    {
        QListWidgetItem* item = ui->listWidget_friends->item(i);
        QWidget* itemWidget = ui->listWidget_friends->itemWidget(item);
        FriendItem* friendItem = dynamic_cast<FriendItem*>(itemWidget);
        MsgBuilder::UserData data = friendItem->getUserData();
        if(data.id == user.id)
        {
            ui->listWidget_friends->removeItemWidget(item);     
            delete item;
        }
    }
}

void MainWindow::createGroupReturn(QString jsonStr)
{
    int groupId = MsgBuilder::parseCreateGroupReturn(jsonStr);
    createGroupDialog->showGroupId(groupId);
}

void MainWindow::searchGroupReturn(QString jsonStr)
{
    vector<int> ids;
    vector<QString> names;
    MsgBuilder::parseSearchGroupReturn(jsonStr, ids, names);
    searchGroupDialog->setGroups(ids, names);
}

void MainWindow::joinGroupReturn(QString jsonStr)
{
    vector<MsgBuilder::UserData> groupFriends;
    int groupId = MsgBuilder::parseJoinGroupReturn(jsonStr, groupFriends);
    if(groupChats.count(groupId) == 0)
    {
        return;
    }
    groupChats[groupId]->addGroupFriends(groupFriends);
}

void MainWindow::userJoinGroup(QString jsonStr)
{
    MsgBuilder::UserData groupFriend;
    int groupId;
    MsgBuilder::parseUserJoinGroup(jsonStr, groupFriend, groupId);
    GroupChatDialog* dialog = groupChats[groupId];
    dialog->addGroupFriend(groupFriend);
}

void MainWindow::userLeaveGroup(QString jsonStr)
{
    int groupId;
    MsgBuilder::UserData user;
    MsgBuilder::parseUserLeaveGroup(jsonStr, user, groupId);
    GroupChatDialog* dialog = groupChats[groupId];
    dialog->removeGroupFriend(user);
}

void MainWindow::receiveGroupMsg(QString jsonStr)
{
    int groupId;
    MsgBuilder::UserData user;
    QString msg = MsgBuilder::parseReceiveGroupMsg(jsonStr, groupId, user);
    GroupChatDialog* dialog = groupChats[groupId];
    dialog->addReceiveMsg(user, msg);
}

void MainWindow::readyRead()
{
    QString data = QString::fromLocal8Bit(socket->readAll());
    switch(MsgBuilder::msgType(data))
    {
    case MsgBuilder::userOnline:{
        MsgBuilder::UserData user = MsgBuilder::parseUserOnline(data);
        addFriendsList(user);
        break;}
    case MsgBuilder::userOffline:{
        MsgBuilder::UserData user = MsgBuilder::parseUserOffline(data);
        removeFromFriendList(user);
        break;}
    case MsgBuilder::receiveMsg:
        receiveMsg(data);
        break;
    case MsgBuilder::createGroupReturn:
        createGroupReturn(data);
        break;
    case MsgBuilder::searchGroupReturn:
        searchGroupReturn(data);
        break;
    case MsgBuilder::joinGroupReturn:
        joinGroupReturn(data);
        break;
    case MsgBuilder::userJoinGroup:
        userJoinGroup(data);
        break;
    case MsgBuilder::userLeaveGroup:
        userLeaveGroup(data);
        break;
    case MsgBuilder::receiveGroupMsg:
        receiveGroupMsg(data);
        break;
    }
}

void MainWindow::initSocket()
{
    socket = HHTcpSocket::getInstance();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void MainWindow::initFriendsList(vector<MsgBuilder::UserData> friends)
{
    for(int i = 0;i < friends.size();i++)
    {
        addFriendsList(friends[i]);
    }
}

void MainWindow::receiveMsg(QString jsonStr)
{
    MsgBuilder::UserData guest;
    MsgBuilder::UserData host;
    QString msg = MsgBuilder::parseReceiveMsg(jsonStr, guest, host);
    //如果打开聊天窗口，直接显示
    if(chatFriends.count(guest) == 1)
    {
        chatFriends[guest].dialog->addReceiveMsg(guest, msg);
        return;
    }
    
    //如果没打开聊天窗口，信息添加到FriendItem中
    for(int i = 0;i < ui->listWidget_friends->count();i++)
    {
        QListWidgetItem* item = ui->listWidget_friends->item(i);
        QWidget* itemWidget = ui->listWidget_friends->itemWidget(item);
        FriendItem* friendItem = dynamic_cast<FriendItem*>(itemWidget);
        MsgBuilder::UserData data = friendItem->getUserData();
        if(data.id == guest.id)//找到发送信息好友
        {
            friendItem->addWaitMsg(msg);
            friendItem->msgAlarm(true);
        }
    }
}

//关闭聊天窗口
void MainWindow::rejected()
{
    ChatDialog* dialog = dynamic_cast<ChatDialog*>(QObject::sender());
    chatFriends.erase(chatFriends.find(dialog->getuserData()));
}

void MainWindow::createGroupDialogRejected()
{
    createGroupDialog = 0;
}

void MainWindow::searchGroupDialogRejected()
{
    searchGroupDialog = 0;
}

void MainWindow::groupChatDialogRejected()
{
    GroupChatDialog* dialog = dynamic_cast<GroupChatDialog*>(QObject::sender());
    QString data = MsgBuilder::buildLeaveGroup(socket->getHostData(), dialog->getGroupId());
    socket->write(data.toLocal8Bit());
    int groupId = dialog->getGroupId();
    groupChats.erase(groupId);
}

void MainWindow::createGroupChatDialog(int id, QString name)
{
    if(groupChats.count(id) == 1)//一个群聊只能打开一个窗口
        return;
    groupChats[id] = new GroupChatDialog(id, name, this);
    groupChats[id]->show();
    connect(groupChats[id], SIGNAL(rejected()), this, SLOT(groupChatDialogRejected()));
    
    //发送加入群聊信息
    QString data = MsgBuilder::buildJoinGroup(socket->getHostData(), id);
    socket->write(data.toLocal8Bit());
}

//打开聊天窗口
void MainWindow::on_listWidget_friends_itemDoubleClicked(QListWidgetItem *item)
{
    QWidget* widget = ui->listWidget_friends->itemWidget(item);
    FriendItem* friendItem = dynamic_cast<FriendItem*>(widget);
    MsgBuilder::UserData user = friendItem->getUserData();
    if(chatFriends.count(user) == 1)//已经打开聊天窗口不再打开
    {
        return;
    }
    //创建聊天界面
    QStringList waitMsgs = friendItem->getWaitMsgs();
    qDebug()<<"double  "<<friendItem->getUserData().headId;

    ChatDialog* dialog = new ChatDialog(waitMsgs, friendItem, friendItem->getUserData(), this);
    connect(dialog, SIGNAL(rejected()), this, SLOT(rejected()));
    dialog->show();
    friendItem->msgAlarm(false);
    //保存聊天信息
    ItemAndDialog iad;
    iad.dialog = dialog;
    iad.item = friendItem;
    chatFriends[user] = iad;
}

void MainWindow::on_pushButton_createGroup_clicked()
{
    if(createGroupDialog != 0)
    {
        return;
    }
    createGroupDialog = new CreateGroupDialog(this);
    connect(createGroupDialog, SIGNAL(rejected()), this, SLOT(createGroupDialogRejected()));
    createGroupDialog->show();
}

void MainWindow::on_pushButton_searchGroup_clicked()
{
    if(searchGroupDialog != 0)
    {
        return;
    }
    searchGroupDialog = new SearchGroupDialog(this);
    searchGroupDialog->show();
    
    QString data = MsgBuilder::buildSearchGroup();
    socket->write(data.toLocal8Bit());
    
    connect(searchGroupDialog, SIGNAL(rejected()), this, SLOT(searchGroupDialogRejected()));
    connect(searchGroupDialog, SIGNAL(createGroupChatDialog(int,QString)), this, SLOT(createGroupChatDialog(int,QString)));
}
