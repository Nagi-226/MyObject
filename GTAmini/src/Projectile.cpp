#include "Projectile.h"
#include <iostream>
#include <cmath>

// 构造函数
Projectile::Projectile(ProjectileType type, sf::Vector2f position, sf::Vector2f direction, float speed, int damage, float range)
    : mType(type)
    , mDirection(direction)
    , mSpeed(speed)
    , mDamage(damage)
    , mRange(range)
    , mActive(true)
    , mDistanceTraveled(0.f)
{
    // 设置初始位置
    mPosition = position;
    
    // 根据投射物类型设置属性
    switch (type)
    {
        case ProjectileType::BULLET:
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/bullet.png"))
            {
                std::cerr << "Failed to load bullet texture!" << std::endl;
            }
            
            // 设置碰撞边界
            mBounds.width = 4.f;
            mBounds.height = 4.f;
            break;
            
        case ProjectileType::SHOTGUN_PELLET:
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/shotgun_pellet.png"))
            {
                std::cerr << "Failed to load shotgun pellet texture!" << std::endl;
            }
            
            // 设置碰撞边界
            mBounds.width = 3.f;
            mBounds.height = 3.f;
            break;
            
        case ProjectileType::ROCKET:
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/rocket.png"))
            {
                std::cerr << "Failed to load rocket texture!" << std::endl;
            }
            
            // 设置碰撞边界
            mBounds.width = 8.f;
            mBounds.height = 8.f;
            break;
    }
    
    // 设置精灵
    mSprite.setTexture(mTexture);
    mSprite.setOrigin(mTexture.getSize().x / 2.f, mTexture.getSize().y / 2.f);
    mSprite.setPosition(mPosition);
    
    // 设置旋转角度
    float angle = std::atan2(mDirection.y, mDirection.x) * 180.f / 3.14159f;
    mSprite.setRotation(angle + 90.f); // +90度是因为精灵默认朝上
    mRotation = angle + 90.f;
    
    // 更新碰撞边界位置
    mBounds.left = mPosition.x - mBounds.width / 2.f;
    mBounds.top = mPosition.y - mBounds.height / 2.f;
}

// 析构函数
Projectile::~Projectile()
{
}

// 更新投射物状态
void Projectile::update(sf::Time deltaTime)
{
    // 只有在活跃状态下才更新
    if (mActive)
    {
        // 计算移动距离
        float distance = mSpeed * deltaTime.asSeconds();
        
        // 更新位置
        mPosition += mDirection * distance;
        
        // 更新精灵位置
        mSprite.setPosition(mPosition);
        
        // 更新碰撞边界位置
        mBounds.left = mPosition.x - mBounds.width / 2.f;
        mBounds.top = mPosition.y - mBounds.height / 2.f;
        
        // 更新已飞行距离
        mDistanceTraveled += distance;
        
        // 检查是否超出射程
        if (mDistanceTraveled >= mRange)
        {
            mActive = false;
        }
    }
}

// 渲染投射物
void Projectile::render(sf::RenderWindow& window)
{
    // 只有在活跃状态下才渲染
    if (mActive)
    {
        window.draw(mSprite);
    }
}

// 获取投射物类型
ProjectileType Projectile::getType() const
{
    return mType;
}

// 获取投射物伤害
int Projectile::getDamage() const
{
    return mDamage;
}

// 获取投射物是否活跃
bool Projectile::isActive() const
{
    return mActive;
}

// 设置投射物是否活跃
void Projectile::setActive(bool active)
{
    mActive = active;
}

// 获取投射物已经飞行的距离
float Projectile::getDistanceTraveled() const
{
    return mDistanceTraveled;
}