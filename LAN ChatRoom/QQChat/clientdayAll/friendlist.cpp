#include "friendlist.h"
#include "ui_friendlist.h"

//参数1 所有在线用户信息
FriendList::FriendList(QString jsonStr,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendList)
{
    ui->setupUi(this);


    //1.获取套接字
    socket = userSocket::getInstance();

    //2.信号槽函数连接
    //当套接字有数据 socket发出信号readyRead
    connect(socket,&userSocket::readyRead,this,&FriendList::on_readyRead_slot);

    //当连接断开 套接字会发出信号disconnected
    connect(socket,&userSocket::disconnected,this,&FriendList::on_disconnected_slot);

    //3.初始化好友列表
    initFriendList(jsonStr);


    //4.设置本窗口标题
    setWindowTitle(socket->getData().nickName);

}

void FriendList::initFriendList(QString jsonStr)
{
    //1.解析jsonStr 获取所有在线用户信息
    UserData self;//自己信息
    vector<UserData> others;//所有其他在线用户的信息
    QListWidgetItem *it=NULL;
    OneFriend *f=NULL;

    MsgBuilder::parseUserLoginSucMsg(jsonStr,self,others);

    //2.遍历vector容器 获取所有其他在线用户信息 实例化好友
    for(int i=0;i<others.size();i++)
    {
        //实例化自定义控件--好友
        f = new OneFriend(others[i],ui->listWidget);

        //QListWidget每一项都是 QListWidgetItem  实例化QListWidgetItem对象 指定父窗口
        it = new QListWidgetItem(ui->listWidget);

        //调整it的尺寸适应f大小
        it->setSizeHint(f->size());

        //设置QListWidgetItem 将QListWidgetItem与自定义空间绑定
        ui->listWidget->setItemWidget(it,f);
    }

    //套接字保存当前用户信息
    socket->setData(self);


}

void FriendList::dealUserOnline(QString jsonStr)
{
    UserData udata;//上线用户
    //解析
    MsgBuilder::parseUserOnlineMsg(jsonStr,udata);

    //更新好友列表 添加好友
    QListWidgetItem *it=NULL;
    OneFriend *f=NULL;

    //实例化自定义控件--好友
    f = new OneFriend(udata,ui->listWidget);

    //QListWidget每一项都是 QListWidgetItem  实例化QListWidgetItem对象 指定父窗口
    it = new QListWidgetItem(ui->listWidget);

    //调整it的尺寸适应f大小
    it->setSizeHint(f->size());

    //设置QListWidgetItem 将QListWidgetItem与自定义空间绑定
    ui->listWidget->setItemWidget(it,f);
}

void FriendList::dealUserOffLine(QString jsonOff )
{
    QListWidgetItem *item = NULL;
    QWidget *wItem = NULL;
    OneFriend *f = NULL;
    UserData udata,userOff;

    //解析 获得下线用户数据 存入useroff
    MsgBuilder::parseUserOfflineMsg(jsonOff,userOff);

    //得到行号  count ：好友列表中好友个数（QListWidgetItem个数）
    for(int i=0;i<ui->listWidget->count();i++)
    {
        //根据行号得到item
       item =  ui->listWidget->item(i);

       //得到oneFriend
       //首先得到item窗口
       wItem = ui->listWidget->itemWidget(item);

       f = dynamic_cast<OneFriend*>(wItem);//强转

       //遍历到的好友信息
       udata = f->getUserData();

    //判断是否为下线好友 如果是 就删除
       if(udata.userId==userOff.userId)
       {
           //删除下线好友
           ui->listWidget->takeItem(i);//takeItem 带走 删除
           delete item;

       }

    }


}

void FriendList::dealUserReceiveMsg(QString jsonStr)
{
    QString msg;
    UserData from,to,udata;
    QListWidgetItem *item=NULL;
    QWidget *w = NULL;
    OneFriend *f = NULL;

    //解析
    MsgBuilder::parseUserReceiveMsg(jsonStr,from,to,msg);

    //1.判断私聊窗口是否已经打开
    //如果私聊窗口没有打开 设置未读消息 oneFriend
    if(usersDialog.count(from)==0)
    {
        //遍历查找聊天好友
        for(int i=0;i<ui->listWidget->count();i++)
        {
            //根据行号得到列表项
            item = ui->listWidget->item(i);

            //根据列表项得到窗口
            w = ui->listWidget->itemWidget(item);
            f= dynamic_cast<OneFriend*>(w);

            //得到好友信息
            udata = f->getUserData();

            //判断该好友是否为信息发出者
            if(udata.userId==from.userId)
            {

                //添加未读消息到好友列表项
                f->addWaitMsg(msg);

                //显示提示语:"新消息"
                f->msgAlarm(true);
            }
        }
        return;
    }

    //如果私聊窗口已经打开 窗口显示聊天消息
    //通过用户信息 获取私聊窗口
    ChatDialog *cd =  usersDialog[from];
    cd->addUserChatMsg(msg);


}


void FriendList::on_readyRead_slot()
{
    //接收套接字消息
    QByteArray bdata= socket->readAll();
    QString jsonStr = QString::fromLocal8Bit(bdata);

    //匹配处理
    switch(MsgBuilder::getType(jsonStr))
    {
    case MsgBuilder::userOnline:
        dealUserOnline(jsonStr);
        break;
    case MsgBuilder::userOffline://下线
        qDebug()<<"friendlist read:"<<jsonStr;
        dealUserOffLine(jsonStr);
        break;
    case MsgBuilder::userReceiveMsg://有新消息

        dealUserReceiveMsg(jsonStr);
        break;
    }

}

void FriendList::on_disconnected_slot()
{

}


FriendList::~FriendList()
{
    delete ui;
}

//好友界面双击项目(QListWidgetItem) 自动触发
void FriendList::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //1.获取点击的列表项好友信息
     QWidget *w  = ui->listWidget->itemWidget(item);
     OneFriend *f = dynamic_cast<OneFriend*>(w);
     UserData udata = f->getUserData();//获取被点击的好友信息

    //2.判断该好友私聊窗口是否已经打开
    if(usersDialog.count(udata)==1)
    {
        return;//不要弹出聊天窗口
    }

    //3.获取好友的未读消息

    //字符串集合
    QStringList waitMsg= f->getMsgs();



    //4.弹出私聊窗口
    //参数1 未读消息 参数2 被点击好友信息
   ChatDialog *cd = new ChatDialog(waitMsg,udata,this);

   //信号槽函数连接 信号：私聊窗口关闭 自动发出信号
   connect(cd,&ChatDialog::sigUserDialogClose,this,&FriendList::on_sigUserDialogClose_slot);

   cd->show();


   //5.未读消息提示(新消息)
   f->msgAlarm(false);

   //6.将好友信息 及打开的聊天框插入容器 记录
   usersDialog.insert(pair<UserData,ChatDialog*>(udata,cd));


}

//私聊窗口关闭 触发槽函数
void FriendList::on_sigUserDialogClose_slot(UserData udata)
{
    //私聊对话框管理容器中删除该好友私聊窗口

   // ChatDialog *cd =  usersDialog[udata];
    usersDialog.erase(udata);

}

























