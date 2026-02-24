#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

#include "Entity.h"

// 投射物类型枚举
enum class ProjectileType {
    BULLET,
    SHOTGUN_PELLET,
    ROCKET
};

class Projectile : public Entity {
public:
    // 构造函数和析构函数
    Projectile(ProjectileType type, sf::Vector2f position, sf::Vector2f direction, float speed, int damage, float range);
    ~Projectile();

    // 更新投射物状态
    void update(sf::Time deltaTime) override;
    
    // 渲染投射物
    void render(sf::RenderWindow& window) override;

    // 获取投射物类型
    ProjectileType getType() const;
    
    // 获取投射物伤害
    int getDamage() const;
    
    // 获取投射物是否活跃
    bool isActive() const;
    
    // 设置投射物是否活跃
    void setActive(bool active);
    
    // 获取投射物已经飞行的距离
    float getDistanceTraveled() const;

private:
    // 投射物类型
    ProjectileType mType;
    
    // 投射物精灵
    sf::Sprite mSprite;
    
    // 投射物纹理
    sf::Texture mTexture;
    
    // 投射物方向
    sf::Vector2f mDirection;
    
    // 投射物速度
    float mSpeed;
    
    // 投射物伤害
    int mDamage;
    
    // 投射物射程
    float mRange;
    
    // 投射物是否活跃
    bool mActive;
    
    // 投射物已经飞行的距离
    float mDistanceTraveled;
};

#endif // PROJECTILE_H