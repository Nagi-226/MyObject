/********************************************************************************
** Form generated from reading UI file 'onefriend.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONEFRIEND_H
#define UI_ONEFRIEND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OneFriend
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_head;
    QLabel *label_name;

    void setupUi(QWidget *OneFriend)
    {
        if (OneFriend->objectName().isEmpty())
            OneFriend->setObjectName(QStringLiteral("OneFriend"));
        OneFriend->resize(253, 84);
        gridLayout = new QGridLayout(OneFriend);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_head = new QLabel(OneFriend);
        label_head->setObjectName(QStringLiteral("label_head"));

        horizontalLayout->addWidget(label_head);

        label_name = new QLabel(OneFriend);
        label_name->setObjectName(QStringLiteral("label_name"));

        horizontalLayout->addWidget(label_name);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(OneFriend);

        QMetaObject::connectSlotsByName(OneFriend);
    } // setupUi

    void retranslateUi(QWidget *OneFriend)
    {
        OneFriend->setWindowTitle(QApplication::translate("OneFriend", "Form", 0));
        label_head->setText(QApplication::translate("OneFriend", "TextLabel", 0));
        label_name->setText(QApplication::translate("OneFriend", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class OneFriend: public Ui_OneFriend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONEFRIEND_H
