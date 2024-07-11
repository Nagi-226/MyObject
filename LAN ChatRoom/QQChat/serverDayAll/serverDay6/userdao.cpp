#include "userdao.h"
UserDao * UserDao::instance = new UserDao;
UserDao::UserDao()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("usrData.db");

    db.open();

    QSqlQuery query;//QSqlQuery类负责数据库的增删改查操作

    query.exec("create table if not exists user(\
               userid integer primary key autoincrement,\
               password char(20),\
               nickname char(20),\
               headid integer\
               )");
            //插入一个原始用户因为希望用户的id是用100001开始，所以原始用户的id是100000
            query.exec("insert into user values(100000,'abc123','admin',0)");
}



UserDao::~UserDao()
{
    db.close();//关闭数据库
}

UserDao *UserDao::getInstance()
{
    return instance;
}

bool UserDao::insertUser(UserEntity &e)
{

    QSqlQuery query;

    //1.插入要注册的用户信息

    //prepare可以设置一个带参数的sql语句，？表示参数
    query.prepare("insert into user(password, nickname, headid) values(?,?,?)");

    //bindValue 是给prepare中sql的？绑定参数的  0代表第一个?，1代表第二个?
    query.bindValue(0, e.passwd);
    query.bindValue(1, e.nickName);
    query.bindValue(2, e.headId);
    bool ok = query.exec();//执行绑定参数后的sql语句

    //2.刚刚插入的记录 userId 获取

    query.exec("SELECT LAST_INSERT_ROWID()");
    if(query.next())
    {
        //将查询到的userId 赋值到e中  e是输入参数 也是输出参数（带回数据）
        e.userId = query.record().value(0).toInt();

    }

    return ok;

}

//匹配形参e的用户ID 密码是否都正确 正确则赋值true给ok 表示可以登录
bool UserDao::selectUser(UserEntity &e, bool &ok)
{
    QSqlQuery query;
    QSqlRecord record;
    //查找
    query.prepare("select * from user where userid = ? and password = ?");
    query.bindValue(0, e.userId);
    query.bindValue(1,e.passwd);
    bool ok1 = query.exec();

    if(query.next())
    {
        //登录成功
        ok = true;

        //获取当前用户的nickName headId
        //QSqlRecord中存放了一行的数据
        //获取昵称 headid由e带回
        e.nickName = query.record().value("nickname").toString();
        e.headId= query.record().value("headid").toInt();

    }
    else
    {
        //登录失败
        ok = false;
    }

    return ok1;
}

UserDao::UserDao(const UserDao &)
{

}
