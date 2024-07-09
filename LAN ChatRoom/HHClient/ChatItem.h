#ifndef CHATITEM_H
#define CHATITEM_H

#include <QWidget>
#include <QLabel>

#define ITEM_WIDTH 600
#define HEAD_WIDTH  50
#define HEAD_HEIGHT 50

class ChatItem : public QWidget
{
    Q_OBJECT
    
public:
    enum ChatRole
    {
        host,
        guest
    };
    ChatItem(QString msg, int headId, ChatRole role, QWidget *parent = 0);
    ~ChatItem();
    
private:
    void hostSet();
    void guestSet();
private:
    QLabel* labelMsg;
    QLabel* labelHead;
    
};

#endif // CHATITEM_H
