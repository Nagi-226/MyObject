#ifndef MSGMARK_H
#define MSGMARK_H

#include <QWidget>
#include<QPixmap>//图片类
namespace Ui {
class msgMark;
}

class msgMark : public QWidget
{
    Q_OBJECT

public:
    explicit msgMark(QWidget *parent = 0);
    ~msgMark();

    void setNum(int);//设置未读消息数量

private:
    Ui::msgMark *ui;
};

#endif // MSGMARK_H
