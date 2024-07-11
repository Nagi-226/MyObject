/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QLineEdit *lineEdit_password;
    QLabel *label;
    QPushButton *pushButton_register;
    QLineEdit *lineEdit_nickname;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_id;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName(QStringLiteral("RegisterDialog"));
        RegisterDialog->setWindowModality(Qt::ApplicationModal);
        RegisterDialog->resize(555, 216);
        lineEdit_password = new QLineEdit(RegisterDialog);
        lineEdit_password->setObjectName(QStringLiteral("lineEdit_password"));
        lineEdit_password->setGeometry(QRect(80, 100, 361, 25));
        lineEdit_password->setFocusPolicy(Qt::ClickFocus);
        label = new QLabel(RegisterDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 20, 381, 18));
        pushButton_register = new QPushButton(RegisterDialog);
        pushButton_register->setObjectName(QStringLiteral("pushButton_register"));
        pushButton_register->setGeometry(QRect(320, 150, 112, 34));
        lineEdit_nickname = new QLineEdit(RegisterDialog);
        lineEdit_nickname->setObjectName(QStringLiteral("lineEdit_nickname"));
        lineEdit_nickname->setGeometry(QRect(80, 60, 361, 25));
        label_2 = new QLabel(RegisterDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 60, 81, 18));
        label_3 = new QLabel(RegisterDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 100, 81, 18));
        label_id = new QLabel(RegisterDialog);
        label_id->setObjectName(QStringLiteral("label_id"));
        label_id->setGeometry(QRect(40, 150, 201, 18));

        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QApplication::translate("RegisterDialog", "Dialog", 0));
        label->setText(QApplication::translate("RegisterDialog", "\350\276\223\345\205\245\345\257\206\347\240\201\357\274\214\347\255\211\345\276\205\346\263\250\345\206\214\346\210\220\345\212\237\350\277\224\345\233\236\350\264\246\345\217\267", 0));
        pushButton_register->setText(QApplication::translate("RegisterDialog", "\346\263\250\345\206\214", 0));
        label_2->setText(QApplication::translate("RegisterDialog", "\346\230\265\347\247\260", 0));
        label_3->setText(QApplication::translate("RegisterDialog", "\345\257\206\347\240\201", 0));
        label_id->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
