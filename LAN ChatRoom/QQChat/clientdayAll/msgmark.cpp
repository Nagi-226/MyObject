#include "msgmark.h"
#include "ui_msgmark.h"

msgMark::msgMark(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::msgMark)
{
    ui->setupUi(this);

    this->setVisible(false);//当前对象不可见

    //构造一张图片
    QPixmap head = QPixmap(":/new/prefix1/heads/red.png");

    //设置图片head 根据label_show大小 等比例缩放
    head = head.scaled(ui->label_red->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->label_red->setPixmap(head);//设置图片

    //setNum 设置整数
    ui->label_num->setNum(888);
}

msgMark::~msgMark()
{
    delete ui;
}

void msgMark::setNum(int num)
{
    //如果未读消息为0
    if(num!=0)
    {
        setVisible(true);//当前对象可见
        ui->label_num->setNum(num);
    }
    else if(0==num)
    {
        setVisible(false);
    }
}
