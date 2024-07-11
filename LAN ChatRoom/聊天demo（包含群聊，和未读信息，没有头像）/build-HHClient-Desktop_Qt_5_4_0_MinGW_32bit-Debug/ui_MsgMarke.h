/********************************************************************************
** Form generated from reading UI file 'MsgMarke.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MSGMARKE_H
#define UI_MSGMARKE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MsgMarke
{
public:
    QLabel *label;
    QLabel *label_num;

    void setupUi(QWidget *MsgMarke)
    {
        if (MsgMarke->objectName().isEmpty())
            MsgMarke->setObjectName(QStringLiteral("MsgMarke"));
        MsgMarke->resize(31, 32);
        label = new QLabel(MsgMarke);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 31, 31));
        label->setPixmap(QPixmap(QString::fromUtf8(":/heads/red.png")));
        label->setScaledContents(true);
        label_num = new QLabel(MsgMarke);
        label_num->setObjectName(QStringLiteral("label_num"));
        label_num->setGeometry(QRect(0, 0, 31, 21));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        label_num->setPalette(palette);
        label_num->setAlignment(Qt::AlignCenter);

        retranslateUi(MsgMarke);

        QMetaObject::connectSlotsByName(MsgMarke);
    } // setupUi

    void retranslateUi(QWidget *MsgMarke)
    {
        MsgMarke->setWindowTitle(QApplication::translate("MsgMarke", "Form", 0));
        label->setText(QString());
        label_num->setText(QApplication::translate("MsgMarke", "1", 0));
    } // retranslateUi

};

namespace Ui {
    class MsgMarke: public Ui_MsgMarke {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MSGMARKE_H
