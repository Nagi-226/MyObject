#include "onefriend.h"
#include "ui_onefriend.h"

OneFriend::OneFriend(UserData udata,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OneFriend)
{
    ui->setupUi(this);

    //存储当前上线好友的信息
    this->udata = udata;

    //构造一张图片
    QPixmap head = QPixmap(":/new/prefix1/heads/head29.jpg");

    //设置图片head 根据label_show大小 等比例缩放
    head = head.scaled(ui->label_head->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->label_head->setPixmap(head);//设置图片

    ui->label_name->setText(udata.nickName);

    //实例化未读消息标记 类对象
    m = new msgMark;

    //设置未读消息数量
    m->setNum(waitMsg.size());

    //添加m控件到水平布局
    ui->horizontalLayout->addWidget(m);
}

OneFriend::~OneFriend()
{
    delete ui;
}

void OneFriend::msgAlarm(bool open)
{
    QString name;
    //有未读消息
    if(open)
    {
        name = QString("%1(%2)").arg(udata.nickName).arg("新消息");
        ui->label_name->setText(name);
    }
    else//没有未读消息
    {
         ui->label_name->setText(udata.nickName);
    }

}


void OneFriend::addWaitMsg(QString msg)
{
    //追加未读消息
    waitMsg.append(msg);
    //更新未读消息的数量
    m->setNum(waitMsg.size());
    qDebug()<<"addwait:"<<waitMsg.size();
}

QStringList OneFriend::getMsgs()
{
    QStringList tmp = waitMsg;
    waitMsg.clear();//清除容器

    m->setNum(0);//未读消息清零

    return tmp;
}











