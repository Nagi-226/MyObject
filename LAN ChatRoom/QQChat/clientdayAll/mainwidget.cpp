#include "mainwidget.h"
#include "ui_mainwidget.h"

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    //设置提示
    ui->lineEdt_ip->setPlaceholderText("服务器IP");
    ui->lineEdit_passwd->setPlaceholderText("密码");

    //设置密码最大长度
    ui->lineEdit_passwd->setMaxLength(6);
    //设置非明文显示
    ui->lineEdit_passwd->setEchoMode(QLineEdit::Password);

    ui->lineEdit_usr->setPlaceholderText("账号");


   //取得套接字
    socket = userSocket::getInstance();

    //连接成功 会发送信号connected
    connect(socket,&userSocket::connected,this,&mainWidget::on_connected_slot);

    //如果有消息到来 套接字会发出readyRead信号
    //信号与槽函数连接
    connect(socket,&userSocket::readyRead,this,&mainWidget::on_readyRead_slot);

}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::on_btn_connect_clicked()
{
    QString ip = ui->lineEdt_ip->text();
   // socket->abort();
    socket->connectToHost(QHostAddress(ip),6666);

}

//连接成功 自动调用
void mainWidget::on_connected_slot()
{
    qDebug()<<"连接成功";

    //设置空间是否可用 false 不可用 true可用
   // ui->btn_connect->setEnabled(false);
    // ui->lineEdt_ip->setEnabled(false);

    ui->lineEdit_passwd->setEnabled(true);
    ui->lineEdit_usr->setEnabled(true);

    ui->pushButton_login->setEnabled(true);
    ui->pushButton_register->setEnabled(true);
}

void mainWidget::on_pushButton_register_clicked()
{

    //断开信号与槽函数连接
    disconnect(socket,0,this,0);

    rd = new RegistorDialog;
    rd->exec();//对话框不关闭 会一直阻塞

    //如果有消息到来 套接字会发出readyRead信号
    //信号与槽函数连接
    connect(socket,&userSocket::readyRead,this,&mainWidget::on_readyRead_slot);
}



void mainWidget::on_pushButton_login_clicked()
{
   bool ok;//记录转换是否成功
    //提取界面数据
    QString passwd = ui->lineEdit_passwd->text();

    //toInt 将QString 转为int类型
    int usrId = ui->lineEdit_usr->text().toInt(&ok);

    if(!ok||passwd.isEmpty())
    {
        //消息对话框
        QMessageBox::information(this,"输入有误","账号或密码为空");
        return;
    }

    //将数据发送服务器
    UserData data;
    data.userId = usrId;
    data.passwd  = passwd;

    QString jsonStr = MsgBuilder::buildUserLoginMsg(data);
    QByteArray bdata =  jsonStr.toLocal8Bit();

    socket->write(bdata.data(),bdata.size());

}

void mainWidget::on_readyRead_slot()
{

    //套接字读取数据
    QByteArray bdata= socket->readAll();
    QString jsonStr = QString::fromLocal8Bit(bdata);
    qDebug()<<"readyread.."<<jsonStr;

    switch(MsgBuilder::getType(jsonStr))
    {
        case MsgBuilder::userLoginLose:
            QMessageBox::information(this,"登录失败","用户名或者密码有误");
        break;

    case MsgBuilder::userLoginSuc:
        //qDebug()<<"登录成功"<<jsonStr;
        //显示好友列表


        //断开信号与槽函数连接
        disconnect(socket,0,this,0);

        FriendList *f = new FriendList(jsonStr);
        f->show();

        //关闭当前窗口
        this->close();

        break;
    }
}







