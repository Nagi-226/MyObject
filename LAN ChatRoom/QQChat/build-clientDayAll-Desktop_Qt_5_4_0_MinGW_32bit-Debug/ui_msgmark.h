/********************************************************************************
** Form generated from reading UI file 'msgmark.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MSGMARK_H
#define UI_MSGMARK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_msgMark
{
public:
    QLabel *label_red;
    QLabel *label_num;

    void setupUi(QWidget *msgMark)
    {
        if (msgMark->objectName().isEmpty())
            msgMark->setObjectName(QStringLiteral("msgMark"));
        msgMark->resize(64, 62);
        label_red = new QLabel(msgMark);
        label_red->setObjectName(QStringLiteral("label_red"));
        label_red->setGeometry(QRect(20, 20, 21, 21));
        label_num = new QLabel(msgMark);
        label_num->setObjectName(QStringLiteral("label_num"));
        label_num->setGeometry(QRect(20, 20, 21, 21));
        QFont font;
        font.setFamily(QStringLiteral("Cambria"));
        font.setBold(true);
        font.setWeight(75);
        label_num->setFont(font);
        label_num->setAlignment(Qt::AlignCenter);

        retranslateUi(msgMark);

        QMetaObject::connectSlotsByName(msgMark);
    } // setupUi

    void retranslateUi(QWidget *msgMark)
    {
        msgMark->setWindowTitle(QApplication::translate("msgMark", "Form", 0));
        label_red->setText(QString());
        label_num->setText(QApplication::translate("msgMark", "6", 0));
    } // retranslateUi

};

namespace Ui {
    class msgMark: public Ui_msgMark {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MSGMARK_H
