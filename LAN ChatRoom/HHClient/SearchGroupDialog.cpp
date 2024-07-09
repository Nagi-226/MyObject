#include "SearchGroupDialog.h"
#include "ui_searchgroupdialog.h"

SearchGroupDialog::SearchGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchGroupDialog)
{
    ui->setupUi(this);
}

SearchGroupDialog::~SearchGroupDialog()
{
    delete ui;
}

void SearchGroupDialog::setGroups(vector<int> &ids, vector<QString> &names)
{
    for(int i = 0;i < ids.size();i++)
    {
        QString item = QString("%1 %2").arg(names[i]).arg(ids[i]);
        ui->listWidget->addItem(item);
    }
}

void SearchGroupDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString itemStr = item->text();
    int id = itemStr.split(" ")[1].toInt();
    QString name = itemStr.split(" ")[0];
    emit createGroupChatDialog(id, name);
}
