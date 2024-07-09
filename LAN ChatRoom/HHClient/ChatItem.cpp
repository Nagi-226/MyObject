#include "ChatItem.h"
#include <QDebug>

ChatItem::ChatItem(QString msg, int headId, ChatItem::ChatRole role, QWidget *parent):
    QWidget(parent)
{
    //头像
    labelHead = new QLabel(this);
    qDebug()<<"ChatItem  "<<headId;
    QPixmap head = QPixmap(":/heads/head1.jpg");
    switch(headId)
    {
    case 1:
        head = QPixmap(":/heads/head10.jpg");
        break;
    case 2:
        head = QPixmap(":/heads/head20.jpg");
        break;
    }
    head = head.scaled(QSize(HEAD_WIDTH, HEAD_HEIGHT));
    labelHead->setPixmap(head);
    
    //文本
    labelMsg = new QLabel(this);
    labelMsg->setMaximumWidth(ITEM_WIDTH - HEAD_WIDTH);//设置最大宽度，注意自适应大小时可能小于这个宽度就换行
    labelMsg->setWordWrap(true);//允许自动换行
    labelMsg->setText(msg);
    labelMsg->adjustSize();//文本根据内容自适应，这里注意设置的顺序
    labelMsg->setAlignment(Qt::AlignLeft|Qt::AlignTop);//左上对齐         
    labelMsg->resize(labelMsg->width(), labelMsg->height()*1.2f);//把文本的高度调大些，因为控件加入QListyWidget中后文本会被缩短
    labelMsg->setTextInteractionFlags(Qt::TextSelectableByMouse);//设置鼠标可选中文本
    
    switch(role)
    {
    case host:
        hostSet();
        break;
    case guest:
        guestSet();
        break;
    }
    
    resize(ITEM_WIDTH, labelMsg->height()>HEAD_HEIGHT?labelMsg->height():HEAD_HEIGHT);
}

ChatItem::~ChatItem()
{
    
}

void ChatItem::hostSet()
{
    labelHead->setGeometry(QRect(ITEM_WIDTH - HEAD_WIDTH, 0, HEAD_WIDTH, HEAD_HEIGHT));
    labelMsg->move(ITEM_WIDTH - HEAD_WIDTH-labelMsg->width(), 0);
}

void ChatItem::guestSet()
{
    labelHead->setGeometry(QRect(0, 0, HEAD_WIDTH, HEAD_HEIGHT));
    labelMsg->move(HEAD_WIDTH, 0);
}
