#include "MsgMarke.h"
#include "ui_MsgMarke.h"

MsgMarke::MsgMarke(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MsgMarke)
{
    ui->setupUi(this);
    setVisible(false);
}

MsgMarke::~MsgMarke()
{
    delete ui;
}

void MsgMarke::setNum(int num)
{
    if(num == 0)
    {
        setVisible(false);
    }
    else
    {
        setVisible(true);
        ui->label_num->setNum(num>=99?99:num);
    }
}
