#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "HHTcpSocket.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RegisterDialog(QWidget *parent = 0);
    ~RegisterDialog();
    
    void socketData(QString data);
private slots:
    void on_pushButton_register_clicked();

private:
    Ui::RegisterDialog *ui;
    HHTcpSocket* socket;
};

#endif // REGISTERDIALOG_H
