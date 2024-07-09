#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QListWidget>
#include "ChatItem.h"

class ChatWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget* parent = 0);
    ~ChatWidget();
    void addMsg(QString msg, int headId, ChatItem::ChatRole role);
};

#endif // CHATWIDGET_H
