/********************************************************************************
** Form generated from reading UI file 'friendlist.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDLIST_H
#define UI_FRIENDLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendList
{
public:
    QListWidget *listWidget;

    void setupUi(QWidget *FriendList)
    {
        if (FriendList->objectName().isEmpty())
            FriendList->setObjectName(QStringLiteral("FriendList"));
        FriendList->resize(273, 724);
        listWidget = new QListWidget(FriendList);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(10, 10, 251, 701));

        retranslateUi(FriendList);

        QMetaObject::connectSlotsByName(FriendList);
    } // setupUi

    void retranslateUi(QWidget *FriendList)
    {
        FriendList->setWindowTitle(QApplication::translate("FriendList", "FriendList", 0));
    } // retranslateUi

};

namespace Ui {
    class FriendList: public Ui_FriendList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDLIST_H
