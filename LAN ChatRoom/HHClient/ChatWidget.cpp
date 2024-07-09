#include "ChatWidget.h"
#include <QDebug>

ChatWidget::ChatWidget(QWidget *parent):
    QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::NoSelection);//item不可选中，为了消除鼠标移动到item上的效果
    setMaximumWidth(ITEM_WIDTH);
    setMinimumWidth(ITEM_WIDTH);
}

ChatWidget::~ChatWidget()
{
    
}

void ChatWidget::addMsg(QString msg, int headId, ChatItem::ChatRole role)
{
    ChatItem* item = new ChatItem(msg, headId, role, this);
    qDebug()<<"ChatWidget  "<<headId;
    QListWidgetItem* listItem = new QListWidgetItem(this);
    listItem->setSizeHint(item->size());
    setItemWidget(listItem, item);
}

