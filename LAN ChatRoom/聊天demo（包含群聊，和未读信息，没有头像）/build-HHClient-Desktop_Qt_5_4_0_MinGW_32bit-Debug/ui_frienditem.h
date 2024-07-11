/********************************************************************************
** Form generated from reading UI file 'frienditem.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDITEM_H
#define UI_FRIENDITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendItem
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label_head;
    QLabel *label_name;

    void setupUi(QWidget *FriendItem)
    {
        if (FriendItem->objectName().isEmpty())
            FriendItem->setObjectName(QStringLiteral("FriendItem"));
        FriendItem->resize(400, 76);
        horizontalLayout = new QHBoxLayout(FriendItem);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_head = new QLabel(FriendItem);
        label_head->setObjectName(QStringLiteral("label_head"));
        label_head->setMinimumSize(QSize(50, 50));
        label_head->setMaximumSize(QSize(50, 50));

        horizontalLayout->addWidget(label_head);

        label_name = new QLabel(FriendItem);
        label_name->setObjectName(QStringLiteral("label_name"));

        horizontalLayout->addWidget(label_name);


        retranslateUi(FriendItem);

        QMetaObject::connectSlotsByName(FriendItem);
    } // setupUi

    void retranslateUi(QWidget *FriendItem)
    {
        FriendItem->setWindowTitle(QApplication::translate("FriendItem", "Form", 0));
        label_head->setText(QApplication::translate("FriendItem", "TextLabel", 0));
        label_name->setText(QApplication::translate("FriendItem", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class FriendItem: public Ui_FriendItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDITEM_H
