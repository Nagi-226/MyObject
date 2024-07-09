#ifndef MSGMARKE_H
#define MSGMARKE_H

#include <QWidget>

namespace Ui {
class MsgMarke;
}

class MsgMarke : public QWidget
{
    Q_OBJECT
    
public:
    explicit MsgMarke(QWidget *parent = 0);
    ~MsgMarke();
    
    void setNum(int num);
private:
    Ui::MsgMarke *ui;
};

#endif // MSGMARKE_H
