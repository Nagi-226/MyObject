#include "LoginDialog.h"
#include <QApplication>
#include "GroupChatDialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog w;
    //GroupChatDialog w;
    w.show();
    
    return a.exec();
}
