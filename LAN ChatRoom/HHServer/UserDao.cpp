#include "UserDao.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

UserDao* UserDao::instance = 0;

UserDao *UserDao::getInstance()
{
    if(instance == 0)
    {
        instance = new UserDao;
    }
    return instance;
}

void UserDao::releaseInstance()
{
    delete instance;
}

int UserDao::insertUser(UserEntity &e)
{
    QSqlQuery query;
    query.prepare("insert into user(password, nickname, headid) values(?,?,?)");
    query.bindValue(0, e.password);
    query.bindValue(1, e.nickname);
    query.bindValue(2, e.headId);
    query.exec();
    
    query.exec("SELECT LAST_INSERT_ROWID()");
    int id;
    while(query.next())
    {
        id = query.record().value(0).toInt();
    }
    return id;
}

bool UserDao::isPasswordRight(UserEntity &e)
{
    QSqlQuery query;
    query.prepare("select * from user where id = ? and password = ?");
    query.bindValue(0, e.id);
    query.bindValue(1, e.password);
    query.exec();
    int count = 0;
    while(query.next())
    {
        e.nickname = query.record().value("nickname").toString();
        e.headId = query.record().value("headid").toInt();
        count++;
    }
    return count==1?true:false;
}

UserDao::UserDao()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("hhuser.db");
    db.open();
    
    QSqlQuery query;
    query.exec("create table if not exists user(\
                  id integer primary key autoincrement,\
                  password char(20),\
                  nickname char(20),\
                  headid integer\
                  )");
    //插入初始管理员信息，没什么用，就是为了给id一个比较大的起始数字
    query.exec("insert into user values(100000,'abc123','admin',0)");
}

UserDao::~UserDao()
{
    db.close();
}

