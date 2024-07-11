/********************************************************************************
** Form generated from reading UI file 'creategroupdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEGROUPDIALOG_H
#define UI_CREATEGROUPDIALOG_H

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

class Ui_CreateGroupDialog
{
public:
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_groupId;
    QPushButton *pushButton;

    void setupUi(QDialog *CreateGroupDialog)
    {
        if (CreateGroupDialog->objectName().isEmpty())
            CreateGroupDialog->setObjectName(QStringLiteral("CreateGroupDialog"));
        CreateGroupDialog->resize(400, 300);
        lineEdit = new QLineEdit(CreateGroupDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(130, 80, 201, 25));
        label = new QLabel(CreateGroupDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 80, 81, 18));
        label_groupId = new QLabel(CreateGroupDialog);
        label_groupId->setObjectName(QStringLiteral("label_groupId"));
        label_groupId->setGeometry(QRect(60, 130, 261, 18));
        pushButton = new QPushButton(CreateGroupDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(230, 190, 112, 34));

        retranslateUi(CreateGroupDialog);

        QMetaObject::connectSlotsByName(CreateGroupDialog);
    } // setupUi

    void retranslateUi(QDialog *CreateGroupDialog)
    {
        CreateGroupDialog->setWindowTitle(QApplication::translate("CreateGroupDialog", "Dialog", 0));
        label->setText(QApplication::translate("CreateGroupDialog", "\347\276\244\345\220\215\347\247\260", 0));
        label_groupId->setText(QString());
        pushButton->setText(QApplication::translate("CreateGroupDialog", "\345\210\233\345\273\272", 0));
    } // retranslateUi

};

namespace Ui {
    class CreateGroupDialog: public Ui_CreateGroupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEGROUPDIALOG_H
