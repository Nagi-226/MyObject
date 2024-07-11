/********************************************************************************
** Form generated from reading UI file 'registordialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTORDIALOG_H
#define UI_REGISTORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegistorDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_nickName;
    QSpacerItem *verticalSpacer;
    QLineEdit *lineEdit_passwd2;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *lineEdit_confirmPass;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButton_Register;
    QLabel *label_usrId;

    void setupUi(QDialog *RegistorDialog)
    {
        if (RegistorDialog->objectName().isEmpty())
            RegistorDialog->setObjectName(QStringLiteral("RegistorDialog"));
        RegistorDialog->resize(975, 655);
        layoutWidget = new QWidget(RegistorDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(270, 120, 173, 255));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit_nickName = new QLineEdit(layoutWidget);
        lineEdit_nickName->setObjectName(QStringLiteral("lineEdit_nickName"));

        verticalLayout->addWidget(lineEdit_nickName);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        lineEdit_passwd2 = new QLineEdit(layoutWidget);
        lineEdit_passwd2->setObjectName(QStringLiteral("lineEdit_passwd2"));

        verticalLayout->addWidget(lineEdit_passwd2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        lineEdit_confirmPass = new QLineEdit(layoutWidget);
        lineEdit_confirmPass->setObjectName(QStringLiteral("lineEdit_confirmPass"));

        verticalLayout->addWidget(lineEdit_confirmPass);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        pushButton_Register = new QPushButton(layoutWidget);
        pushButton_Register->setObjectName(QStringLiteral("pushButton_Register"));

        verticalLayout->addWidget(pushButton_Register);

        label_usrId = new QLabel(RegistorDialog);
        label_usrId->setObjectName(QStringLiteral("label_usrId"));
        label_usrId->setGeometry(QRect(520, 360, 72, 15));

        retranslateUi(RegistorDialog);

        QMetaObject::connectSlotsByName(RegistorDialog);
    } // setupUi

    void retranslateUi(QDialog *RegistorDialog)
    {
        RegistorDialog->setWindowTitle(QApplication::translate("RegistorDialog", "Dialog", 0));
        pushButton_Register->setText(QApplication::translate("RegistorDialog", "\346\263\250\345\206\214", 0));
        label_usrId->setText(QApplication::translate("RegistorDialog", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class RegistorDialog: public Ui_RegistorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTORDIALOG_H
