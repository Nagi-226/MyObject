#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "Entity.h"
#include "Weapon.h"

class Vehicle; // 前向声明

// NPC类型枚举
enum class NPCType {
    CIVILIAN,
    POLICE,
    GANGSTER
};

// NPC状态枚举
enum class NPCState {
    IDLE,
    WALKING,
    RUNNING,
    DRIVING,
    ATTACKING,
    FLEEING
};

class NPC : public Entity {
public:
    // 构造函数和析构函数
    NPC(NPCType type, sf::Vector2f position);
    ~NPC();

    // 更新NPC状态
    void update(sf::Time deltaTime) override;
    
    // 渲染NPC
    void render(sf::RenderWindow& window) override;

    // NPC行为
    void walk(sf::Vector2f direction);
    void run(sf::Vector2f direction);
    void attack(Entity* target);
    void flee(Entity* threat);
    void enterVehicle(Vehicle* vehicle);
    void exitVehicle();

    // 获取NPC类型
    NPCType getType() const;
    
    // 获取NPC状态
    NPCState getState() const;
    
    // 设置NPC状态
    void setState(NPCState state);

private:
    // 更新AI行为
    void updateAI(sf::Time deltaTime);

    // NPC类型
    NPCType mType;
    
    // NPC状态
    NPCState mState;
    
    // NPC精灵
    sf::Sprite mSprite;
    
    // NPC纹理
    sf::Texture mTexture;
    
    // NPC速度
    float mSpeed;
    
    // NPC方向
    sf::Vector2f mDirection;
    
    // NPC武器
    std::unique_ptr<Weapon> mWeapon;
    
    // NPC当前驾驶的车辆
    Vehicle* mCurrentVehicle;
    
    // NPC生命值
    int mHealth;
    
    // NPC目标实体
    Entity* mTarget;
    
    // NPC威胁实体
    Entity* mThreat;
    
    // NPC决策计时器
    float mDecisionTimer;
    
    // NPC决策间隔
    float mDecisionInterval;
};

#endif // NPC_H