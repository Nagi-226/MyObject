#ifndef VEHICLE_H
#define VEHICLE_H

#include <SFML/Graphics.hpp>
#include <string>

#include "Entity.h"

class Player; // 前向声明

// 车辆类型枚举
enum class VehicleType {
    SEDAN,
    SPORTS_CAR,
    TRUCK,
    POLICE_CAR
};

class Vehicle : public Entity {
public:
    // 构造函数和析构函数
    Vehicle(VehicleType type, sf::Vector2f position);
    ~Vehicle();

    // 更新车辆状态
    void update(sf::Time deltaTime) override;
    
    // 渲染车辆
    void render(sf::RenderWindow& window) override;

    // 车辆行为
    void accelerate(float amount);
    void brake(float amount);
    void turn(float angle);
    
    // 获取车辆类型
    VehicleType getType() const;
    
    // 获取车辆速度
    float getSpeed() const;
    
    // 获取车辆方向
    sf::Vector2f getDirection() const;
    
    // 设置驾驶员
    void setDriver(Player* driver);
    
    // 获取驾驶员
    Player* getDriver() const;
    
    // 车辆是否被占用
    bool isOccupied() const;
    
    // 车辆损坏程度
    float getDamage() const;
    
    // 增加车辆损坏
    void addDamage(float amount);

private:
    // 车辆类型
    VehicleType mType;
    
    // 车辆精灵
    sf::Sprite mSprite;
    
    // 车辆纹理
    sf::Texture mTexture;
    
    // 车辆速度
    float mSpeed;
    
    // 车辆最大速度
    float mMaxSpeed;
    
    // 车辆加速度
    float mAcceleration;
    
    // 车辆方向
    sf::Vector2f mDirection;
    
    // 车辆转向速度
    float mTurnSpeed;
    
    // 车辆驾驶员
    Player* mDriver;
    
    // 车辆损坏程度 (0-100)
    float mDamage;
};

#endif // VEHICLE_H