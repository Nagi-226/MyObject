#include "UserEntity.h"

UserEntity::UserEntity()
{
    
}

UserEntity::UserEntity(QString password, QString nickname, int headId, int id):
    password(password),
    nickname(nickname),
    headId(headId),
    id(id)
{
    
}

UserEntity::~UserEntity()
{
    
}

