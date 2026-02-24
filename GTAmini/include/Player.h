#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "Entity.h"
#include "Weapon.h"

class Vehicle; // 前向声明

class Player : public Entity {
public:
    // 玩家状态枚举
    enum class State {
        WALKING,
        DRIVING,
        SHOOTING
    };

    // 构造函数和析构函数
    Player();
    ~Player();

    // 更新玩家状态
    void update(sf::Time deltaTime) override;
    
    // 处理输入
    void handleInput(const sf::Event& event);
    
    // 渲染玩家
    void render(sf::RenderWindow& window) override;

    // 玩家行为
    void walk(sf::Vector2f direction);
    void shoot();
    void enterVehicle(Vehicle* vehicle);
    void exitVehicle();

    // 获取玩家状态
    State getState() const;
    
    // 获取玩家位置
    sf::Vector2f getPosition() const;
    
    // 设置玩家位置
    void setPosition(sf::Vector2f position);

private:
    // 玩家状态
    State mState;
    
    // 玩家速度
    float mSpeed;
    
    // 玩家方向
    sf::Vector2f mDirection;
    
    // 玩家精灵
    sf::Sprite mSprite;
    
    // 玩家纹理
    sf::Texture mTexture;
    
    // 玩家武器
    std::unique_ptr<Weapon> mCurrentWeapon;
    
    // 玩家当前驾驶的车辆
    Vehicle* mCurrentVehicle;
    
    // 玩家生命值
    int mHealth;
    
    // 玩家护甲值
    int mArmor;
    
    // 玩家金钱
    int mMoney;
};

#endif // PLAYER_H