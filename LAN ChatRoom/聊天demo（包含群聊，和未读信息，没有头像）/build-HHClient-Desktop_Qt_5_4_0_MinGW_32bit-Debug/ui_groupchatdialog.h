/********************************************************************************
** Form generated from reading UI file 'groupchatdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUPCHATDIALOG_H
#define UI_GROUPCHATDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_GroupChatDialog
{
public:
    QListWidget *listWidget_friends;
    QTextEdit *textEdit;
    QPushButton *pushButton_brow;
    QPushButton *pushButton_3;
    QPushButton *pushButton_send;

    void setupUi(QDialog *GroupChatDialog)
    {
        if (GroupChatDialog->objectName().isEmpty())
            GroupChatDialog->setObjectName(QStringLiteral("GroupChatDialog"));
        GroupChatDialog->resize(592, 445);
        listWidget_friends = new QListWidget(GroupChatDialog);
        listWidget_friends->setObjectName(QStringLiteral("listWidget_friends"));
        listWidget_friends->setGeometry(QRect(378, 13, 200, 419));
        listWidget_friends->setMinimumSize(QSize(200, 0));
        textEdit = new QTextEdit(GroupChatDialog);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(13, 249, 200, 183));
        textEdit->setMaximumSize(QSize(16777215, 200));
        pushButton_brow = new QPushButton(GroupChatDialog);
        pushButton_brow->setObjectName(QStringLiteral("pushButton_brow"));
        pushButton_brow->setGeometry(QRect(10, 190, 112, 34));
        pushButton_3 = new QPushButton(GroupChatDialog);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(120, 190, 131, 34));
        pushButton_send = new QPushButton(GroupChatDialog);
        pushButton_send->setObjectName(QStringLiteral("pushButton_send"));
        pushButton_send->setGeometry(QRect(250, 190, 121, 34));

        retranslateUi(GroupChatDialog);

        QMetaObject::connectSlotsByName(GroupChatDialog);
    } // setupUi

    void retranslateUi(QDialog *GroupChatDialog)
    {
        GroupChatDialog->setWindowTitle(QApplication::translate("GroupChatDialog", "Dialog", 0));
        pushButton_brow->setText(QApplication::translate("GroupChatDialog", "\350\241\250\346\203\205", 0));
        pushButton_3->setText(QApplication::translate("GroupChatDialog", "\351\242\204\347\225\231", 0));
        pushButton_send->setText(QApplication::translate("GroupChatDialog", "\345\217\221\351\200\201", 0));
    } // retranslateUi

};

namespace Ui {
    class GroupChatDialog: public Ui_GroupChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPCHATDIALOG_H
