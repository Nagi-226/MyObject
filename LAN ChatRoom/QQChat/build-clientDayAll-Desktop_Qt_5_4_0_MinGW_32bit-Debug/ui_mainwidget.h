/********************************************************************************
** Form generated from reading UI file 'mainwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWidget
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdt_ip;
    QSpacerItem *verticalSpacer;
    QLineEdit *lineEdit_usr;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *lineEdit_passwd;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_login;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_register;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btn_connect;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QWidget *mainWidget)
    {
        if (mainWidget->objectName().isEmpty())
            mainWidget->setObjectName(QStringLiteral("mainWidget"));
        mainWidget->resize(928, 561);
        gridLayout = new QGridLayout(mainWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalSpacer_5 = new QSpacerItem(20, 129, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_5, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(246, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lineEdt_ip = new QLineEdit(mainWidget);
        lineEdt_ip->setObjectName(QStringLiteral("lineEdt_ip"));

        verticalLayout->addWidget(lineEdt_ip);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        lineEdit_usr = new QLineEdit(mainWidget);
        lineEdit_usr->setObjectName(QStringLiteral("lineEdit_usr"));
        lineEdit_usr->setEnabled(false);

        verticalLayout->addWidget(lineEdit_usr);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        lineEdit_passwd = new QLineEdit(mainWidget);
        lineEdit_passwd->setObjectName(QStringLiteral("lineEdit_passwd"));
        lineEdit_passwd->setEnabled(false);

        verticalLayout->addWidget(lineEdit_passwd);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_6);


        verticalLayout_3->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_login = new QPushButton(mainWidget);
        pushButton_login->setObjectName(QStringLiteral("pushButton_login"));
        pushButton_login->setEnabled(false);

        horizontalLayout->addWidget(pushButton_login);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_register = new QPushButton(mainWidget);
        pushButton_register->setObjectName(QStringLiteral("pushButton_register"));
        pushButton_register->setEnabled(false);

        horizontalLayout->addWidget(pushButton_register);


        verticalLayout_3->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        btn_connect = new QPushButton(mainWidget);
        btn_connect->setObjectName(QStringLiteral("btn_connect"));

        verticalLayout_2->addWidget(btn_connect);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        horizontalLayout_2->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout_2, 1, 1, 2, 1);

        horizontalSpacer_3 = new QSpacerItem(245, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 2, 2, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 129, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 3, 1, 1, 1);


        retranslateUi(mainWidget);

        QMetaObject::connectSlotsByName(mainWidget);
    } // setupUi

    void retranslateUi(QWidget *mainWidget)
    {
        mainWidget->setWindowTitle(QApplication::translate("mainWidget", "mainWidget", 0));
        lineEdt_ip->setText(QApplication::translate("mainWidget", "192.168.170.1", 0));
        lineEdit_usr->setText(QApplication::translate("mainWidget", "100006", 0));
        lineEdit_passwd->setText(QApplication::translate("mainWidget", "1", 0));
        pushButton_login->setText(QApplication::translate("mainWidget", "\347\231\273\345\275\225", 0));
        pushButton_register->setText(QApplication::translate("mainWidget", "\346\263\250\345\206\214", 0));
        btn_connect->setText(QApplication::translate("mainWidget", "\350\277\236\346\216\245", 0));
    } // retranslateUi

};

namespace Ui {
    class mainWidget: public Ui_mainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
