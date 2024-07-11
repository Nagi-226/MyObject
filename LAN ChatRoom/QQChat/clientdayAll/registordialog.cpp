#include "registordialog.h"
#include "ui_registordialog.h"

RegistorDialog::RegistorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistorDialog)
{
    ui->setupUi(this);

    //取得套接字
     socket = userSocket::getInstance();

     //信号与槽函数连接
     connect(socket,&userSocket::readyRead,this,&RegistorDialog::on_readyRead_slot);


     //设置提示语
    ui->lineEdit_nickName->setPlaceholderText("昵称");
    ui->lineEdit_passwd2->setPlaceholderText("密码");

    //非明文显示密码
    ui->lineEdit_passwd2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_confirmPass->setPlaceholderText("确认密码");
    ui->lineEdit_confirmPass->setEchoMode(QLineEdit::Password);
}




RegistorDialog::~RegistorDialog()
{
    delete ui;
}

void RegistorDialog::closeEvent(QCloseEvent *e)
{

    //断开信号与槽函数连接
    disconnect(socket,0,this,0);
}

void RegistorDialog::on_pushButton_Register_clicked()
{
    QString msg;
    //提取单行文本编辑器的文本
    QString nickName = ui->lineEdit_nickName->text();
    QString passwd = ui->lineEdit_passwd2->text();
    QString confirm = ui->lineEdit_confirmPass->text();

    //输入的错误处理
    if(nickName.isEmpty()||passwd.isEmpty()||confirm.isEmpty()||passwd!=confirm)
    {
        //错误提示
        //QMessageBox::information(this,"出错了","知道错哪了吗?");
        QMessageBox::question(this,"出错了","知道错哪了吗?");//参数1 父窗口  参数2：标题 参数3：显示文本

        return ;

    }
    else
    {


             UserData userData;
             userData.nickName = nickName;
             userData.passwd = passwd;
             userData.headId = 123;//头像只是先预留数据接口

             //构建json串

             QString jsonStr = MsgBuilder::buildUserRegisterMsg(userData);

             QByteArray bdata = jsonStr.toLocal8Bit();

            //写数据
             socket->write(bdata.data(),bdata.size());

    }


}

void RegistorDialog::on_readyRead_slot()
{
    //接收套接字数据
    QByteArray bdata =  socket->readAll();

    QString jsonStr = QString::fromLocal8Bit(bdata);

    //解析jsonstr
    UserData udata;
    MsgBuilder::parseUserRegisterReturnMsg(jsonStr,udata);

    //获取userId
     if(MsgBuilder::getType(jsonStr)==MsgBuilder::userRegisterReturn)
    {
         //设置userId到文本框
        ui->label_usrId->setNum(udata.userId);
    }
}






















