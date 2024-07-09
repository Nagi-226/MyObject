#include "FriendItem.h"
#include "ui_frienditem.h"

FriendItem::FriendItem(MsgBuilder::UserData user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendItem),
    user(user)
{
    ui->setupUi(this);
    QPixmap head = QPixmap(":/heads/head1.jpg");
    switch(user.headId)
    {
    case 1:
        head = QPixmap(":/heads/head10.jpg");
        break;
    case 2:
        head = QPixmap(":/heads/head20.jpg");
        break;
    }
    head = head.scaled(ui->label_head->size());
    ui->label_head->setPixmap(head);
    
    QString showName = QString("%1(%2)").arg(user.nickname).arg(user.id);
    ui->label_name->setText(showName);
    
    mm = new MsgMarke(this);
    ui->horizontalLayout->addWidget(mm);
}

FriendItem::~FriendItem()
{
    delete ui;
}

QStringList FriendItem::getWaitMsgs()
{
    QStringList temp = waitMsgs;
    waitMsgs.clear();
    mm->setNum(0);
    return temp;
}

void FriendItem::msgAlarm(bool open)
{
    if(open)
    {
        QString showName = QString("(新消息)%1(%2)").arg(user.nickname).arg(user.id);
        ui->label_name->setText(showName);
    }
    else
    {
        QString showName = QString("%1(%2)").arg(user.nickname).arg(user.id);
        ui->label_name->setText(showName);
    }
}
      

