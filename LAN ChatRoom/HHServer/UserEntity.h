#ifndef USERENTITY_H
#define USERENTITY_H

#include <QString>

//这个类对应了表中的每个字段
struct UserEntity
{
public:
    UserEntity();
    UserEntity(QString password, QString nickname, int headId, int id = 0);
    ~UserEntity();
    
    int id;
    QString password;
    QString nickname;
    int headId;
};

#endif // USERENTITY_H
