#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
public:
    // 构造函数和析构函数
    Entity();
    virtual ~Entity();

    // 更新实体状态
    virtual void update(sf::Time deltaTime) = 0;
    
    // 渲染实体
    virtual void render(sf::RenderWindow& window) = 0;

    // 碰撞检测
    bool collidesWith(const Entity& other) const;
    
    // 获取实体位置
    sf::Vector2f getPosition() const;
    
    // 设置实体位置
    void setPosition(sf::Vector2f position);
    
    // 获取实体碰撞边界
    sf::FloatRect getBounds() const;

protected:
    // 实体位置
    sf::Vector2f mPosition;
    
    // 实体旋转角度
    float mRotation;
    
    // 实体碰撞边界
    sf::FloatRect mBounds;
};

#endif // ENTITY_H