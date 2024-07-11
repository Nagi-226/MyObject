#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include"usersocket.h"
#include<QHostAddress>
#include<QString>
#include<QDebug>
#include"registordialog.h"
#include<QMessageBox>//消息对话框
#include"friendlist.h"

namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = 0);
    ~mainWidget();

private slots:
    void on_btn_connect_clicked();
    void on_connected_slot();

    void on_pushButton_register_clicked();

    void on_pushButton_login_clicked();
    void on_readyRead_slot();

private:
      Ui::mainWidget *ui;
    userSocket *socket;
    RegistorDialog *rd;

};

#endif // MAINWIDGET_H
