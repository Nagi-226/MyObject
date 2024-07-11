#ifndef REGISTORDIALOG_H
#define REGISTORDIALOG_H

#include <QDialog>
#include<QMessageBox>//消息框
#include<QByteArray>
#include"usersocket.h"
#include"msgbuilder.h"
#include<QCloseEvent>


namespace Ui {
class RegistorDialog;
}

class RegistorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistorDialog(QWidget *parent = 0);
    ~RegistorDialog();

protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_pushButton_Register_clicked();
    void on_readyRead_slot();


private:
    Ui::RegistorDialog *ui;
    userSocket *socket;

};

#endif // REGISTORDIALOG_H
