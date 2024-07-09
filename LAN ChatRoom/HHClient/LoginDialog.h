#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "HHTcpSocket.h"
#include "Publicdef.h"
#include <QHostAddress>
#include "RegisterDialog.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    
private slots:
    void on_pushButton_connect_clicked();
    
    void on_pushButton_login_clicked();
    
    void on_pushButton_register_clicked();
    
    void connected();
    void readyRead();
private:
    void toMainwindow(QString data);
    void closeEvent(QCloseEvent *e);
private:
    Ui::LoginDialog *ui;
    HHTcpSocket* socket;
    bool loginSuc = false;
    RegisterDialog* registerDialog;
};

#endif // LOGINDIALOG_H
