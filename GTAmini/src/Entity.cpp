#include "Entity.h"

// 构造函数
Entity::Entity()
    : mPosition(0.f, 0.f)
    , mRotation(0.f)
    , mBounds(0.f, 0.f, 0.f, 0.f)
{
}

// 析构函数
Entity::~Entity()
{
}

// 碰撞检测
bool Entity::collidesWith(const Entity& other) const
{
    return mBounds.intersects(other.getBounds());
}

// 获取实体位置
sf::Vector2f Entity::getPosition() const
{
    return mPosition;
}

// 设置实体位置
void Entity::setPosition(sf::Vector2f position)
{
    mPosition = position;
    
    // 更新碰撞边界位置
    mBounds.left = position.x;
    mBounds.top = position.y;
}

// 获取实体碰撞边界
sf::FloatRect Entity::getBounds() const
{
    return mBounds;
}