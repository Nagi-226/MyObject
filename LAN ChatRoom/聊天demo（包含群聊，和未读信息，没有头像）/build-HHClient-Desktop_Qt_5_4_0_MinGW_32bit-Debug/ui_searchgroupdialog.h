/********************************************************************************
** Form generated from reading UI file 'searchgroupdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHGROUPDIALOG_H
#define UI_SEARCHGROUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SearchGroupDialog
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;

    void setupUi(QDialog *SearchGroupDialog)
    {
        if (SearchGroupDialog->objectName().isEmpty())
            SearchGroupDialog->setObjectName(QStringLiteral("SearchGroupDialog"));
        SearchGroupDialog->resize(400, 510);
        verticalLayout = new QVBoxLayout(SearchGroupDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(SearchGroupDialog);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);


        retranslateUi(SearchGroupDialog);

        QMetaObject::connectSlotsByName(SearchGroupDialog);
    } // setupUi

    void retranslateUi(QDialog *SearchGroupDialog)
    {
        SearchGroupDialog->setWindowTitle(QApplication::translate("SearchGroupDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class SearchGroupDialog: public Ui_SearchGroupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHGROUPDIALOG_H
