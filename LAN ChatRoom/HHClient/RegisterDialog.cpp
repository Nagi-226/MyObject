#include "RegisterDialog.h"
#include "ui_registerdialog.h"
#include "MsgBuilder.h"
#include <QDebug>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    socket = HHTcpSocket::getInstance();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::socketData(QString data)
{
    if(MsgBuilder::msgType(data) == MsgBuilder::registerUserReturn)
    {
        int id = MsgBuilder::parseRegisterUserReturnMsg(data);
        ui->label_id->setNum(id);
    }
}

void RegisterDialog::on_pushButton_register_clicked()
{
    QString sendData = MsgBuilder::buildRegisterUserMsg(ui->lineEdit_password->text(), 0, ui->lineEdit_nickname->text());
    int ret = socket->write(sendData.toLocal8Bit());
}


