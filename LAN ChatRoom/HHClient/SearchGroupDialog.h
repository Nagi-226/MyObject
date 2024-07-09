#ifndef SEARCHGROUPDIALOG_H
#define SEARCHGROUPDIALOG_H

#include <QDialog>
#include <vector>
#include <QListWidgetItem>

using namespace std;

namespace Ui {
class SearchGroupDialog;
}

class SearchGroupDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SearchGroupDialog(QWidget *parent = 0);
    ~SearchGroupDialog();
    
    void setGroups(vector<int>& ids, vector<QString>& names);
private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    
signals:
    void createGroupChatDialog(int id, QString name);
private:
    Ui::SearchGroupDialog *ui;
};

#endif // SEARCHGROUPDIALOG_H
