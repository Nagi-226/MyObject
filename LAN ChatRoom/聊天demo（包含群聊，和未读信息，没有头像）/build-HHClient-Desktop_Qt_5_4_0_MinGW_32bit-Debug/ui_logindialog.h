/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLabel *label;
    QLineEdit *lineEdit_serverIp;
    QPushButton *pushButton_connect;
    QLineEdit *lineEdit_id;
    QLineEdit *lineEdit_password;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_login;
    QPushButton *pushButton_register;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName(QStringLiteral("LoginDialog"));
        LoginDialog->resize(682, 183);
        gridLayout = new QGridLayout(LoginDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_3 = new QLabel(LoginDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label = new QLabel(LoginDialog);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit_serverIp = new QLineEdit(LoginDialog);
        lineEdit_serverIp->setObjectName(QStringLiteral("lineEdit_serverIp"));

        gridLayout->addWidget(lineEdit_serverIp, 0, 1, 1, 1);

        pushButton_connect = new QPushButton(LoginDialog);
        pushButton_connect->setObjectName(QStringLiteral("pushButton_connect"));

        gridLayout->addWidget(pushButton_connect, 0, 2, 1, 1);

        lineEdit_id = new QLineEdit(LoginDialog);
        lineEdit_id->setObjectName(QStringLiteral("lineEdit_id"));
        lineEdit_id->setEnabled(false);

        gridLayout->addWidget(lineEdit_id, 1, 1, 1, 1);

        lineEdit_password = new QLineEdit(LoginDialog);
        lineEdit_password->setObjectName(QStringLiteral("lineEdit_password"));
        lineEdit_password->setEnabled(false);

        gridLayout->addWidget(lineEdit_password, 2, 1, 1, 1);

        label_2 = new QLabel(LoginDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_login = new QPushButton(LoginDialog);
        pushButton_login->setObjectName(QStringLiteral("pushButton_login"));
        pushButton_login->setEnabled(false);

        horizontalLayout->addWidget(pushButton_login);

        pushButton_register = new QPushButton(LoginDialog);
        pushButton_register->setObjectName(QStringLiteral("pushButton_register"));
        pushButton_register->setEnabled(false);

        horizontalLayout->addWidget(pushButton_register);


        gridLayout->addLayout(horizontalLayout, 3, 1, 1, 1);


        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QApplication::translate("LoginDialog", "Dialog", 0));
        label_3->setText(QApplication::translate("LoginDialog", "\345\257\206\347\240\201", 0));
        label->setText(QApplication::translate("LoginDialog", "\346\234\215\345\212\241\345\231\250ip", 0));
        lineEdit_serverIp->setText(QApplication::translate("LoginDialog", "192.168.31.196", 0));
        pushButton_connect->setText(QApplication::translate("LoginDialog", "connect", 0));
        label_2->setText(QApplication::translate("LoginDialog", "\350\264\246\345\217\267", 0));
        pushButton_login->setText(QApplication::translate("LoginDialog", "\347\231\273\345\275\225", 0));
        pushButton_register->setText(QApplication::translate("LoginDialog", "\346\263\250\345\206\214", 0));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
