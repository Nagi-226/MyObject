#include "Vehicle.h"
#include "Player.h"
#include <iostream>
#include <cmath>

// 构造函数
Vehicle::Vehicle(VehicleType type, sf::Vector2f position)
    : mType(type)
    , mSpeed(0.f)
    , mDirection(1.f, 0.f)
    , mDriver(nullptr)
    , mDamage(0.f)
{
    // 设置初始位置
    mPosition = position;
    
    // 根据车辆类型设置属性
    switch (type)
    {
        case VehicleType::SEDAN:
            mMaxSpeed = 300.f;
            mAcceleration = 200.f;
            mTurnSpeed = 180.f;
            
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/sedan.png"))
            {
                std::cerr << "Failed to load sedan texture!" << std::endl;
            }
            break;
            
        case VehicleType::SPORTS_CAR:
            mMaxSpeed = 400.f;
            mAcceleration = 300.f;
            mTurnSpeed = 240.f;
            
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/sports_car.png"))
            {
                std::cerr << "Failed to load sports car texture!" << std::endl;
            }
            break;
            
        case VehicleType::TRUCK:
            mMaxSpeed = 200.f;
            mAcceleration = 150.f;
            mTurnSpeed = 120.f;
            
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/truck.png"))
            {
                std::cerr << "Failed to load truck texture!" << std::endl;
            }
            break;
            
        case VehicleType::POLICE_CAR:
            mMaxSpeed = 350.f;
            mAcceleration = 250.f;
            mTurnSpeed = 210.f;
            
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/police_car.png"))
            {
                std::cerr << "Failed to load police car texture!" << std::endl;
            }
            break;
    }
    
    // 设置精灵
    mSprite.setTexture(mTexture);
    mSprite.setOrigin(mTexture.getSize().x / 2.f, mTexture.getSize().y / 2.f);
    mSprite.setPosition(mPosition);
    
    // 设置碰撞边界
    mBounds.width = mTexture.getSize().x * 0.8f;
    mBounds.height = mTexture.getSize().y * 0.8f;
    mBounds.left = mPosition.x - mBounds.width / 2.f;
    mBounds.top = mPosition.y - mBounds.height / 2.f;
}

// 析构函数
Vehicle::~Vehicle()
{
}

// 更新车辆状态
void Vehicle::update(sf::Time deltaTime)
{
    // 更新车辆位置
    mPosition += mDirection * mSpeed * deltaTime.asSeconds();
    
    // 更新精灵位置和旋转
    mSprite.setPosition(mPosition);
    
    // 计算旋转角度
    float angle = std::atan2(mDirection.y, mDirection.x) * 180.f / 3.14159f;
    mSprite.setRotation(angle + 90.f); // +90度是因为精灵默认朝上
    mRotation = angle + 90.f;
    
    // 更新碰撞边界位置
    mBounds.left = mPosition.x - mBounds.width / 2.f;
    mBounds.top = mPosition.y - mBounds.height / 2.f;
    
    // 如果没有驾驶员，车辆会逐渐减速
    if (!mDriver)
    {
        if (mSpeed > 0.f)
        {
            mSpeed -= mAcceleration * 0.5f * deltaTime.asSeconds();
            if (mSpeed < 0.f)
                mSpeed = 0.f;
        }
        else if (mSpeed < 0.f)
        {
            mSpeed += mAcceleration * 0.5f * deltaTime.asSeconds();
            if (mSpeed > 0.f)
                mSpeed = 0.f;
        }
    }
}

// 渲染车辆
void Vehicle::render(sf::RenderWindow& window)
{
    window.draw(mSprite);
}

// 车辆加速
void Vehicle::accelerate(float amount)
{
    // 只有有驾驶员时才能加速
    if (mDriver)
    {
        mSpeed += amount * mAcceleration;
        
        // 限制最大速度
        if (mSpeed > mMaxSpeed)
            mSpeed = mMaxSpeed;
        else if (mSpeed < -mMaxSpeed / 2.f) // 倒车速度限制为最大速度的一半
            mSpeed = -mMaxSpeed / 2.f;
    }
}

// 车辆刹车
void Vehicle::brake(float amount)
{
    // 只有有驾驶员时才能刹车
    if (mDriver)
    {
        if (mSpeed > 0.f)
        {
            mSpeed -= amount * mAcceleration * 2.f; // 刹车比加速快
            if (mSpeed < 0.f)
                mSpeed = 0.f;
        }
        else if (mSpeed < 0.f)
        {
            mSpeed += amount * mAcceleration * 2.f;
            if (mSpeed > 0.f)
                mSpeed = 0.f;
        }
    }
}

// 车辆转向
void Vehicle::turn(float angle)
{
    // 只有有驾驶员且车辆在移动时才能转向
    if (mDriver && mSpeed != 0.f)
    {
        // 转向速度与车速成正比
        float turnFactor = std::abs(mSpeed) / mMaxSpeed;
        
        // 计算新的方向
        float radians = (mRotation - 90.f + angle * turnFactor * mTurnSpeed) * 3.14159f / 180.f;
        mDirection.x = std::cos(radians);
        mDirection.y = std::sin(radians);
        
        // 归一化方向向量
        float length = std::sqrt(mDirection.x * mDirection.x + mDirection.y * mDirection.y);
        if (length != 0.f)
            mDirection /= length;
    }
}

// 获取车辆类型
VehicleType Vehicle::getType() const
{
    return mType;
}

// 获取车辆速度
float Vehicle::getSpeed() const
{
    return mSpeed;
}

// 获取车辆方向
sf::Vector2f Vehicle::getDirection() const
{
    return mDirection;
}

// 设置驾驶员
void Vehicle::setDriver(Player* driver)
{
    mDriver = driver;
}

// 获取驾驶员
Player* Vehicle::getDriver() const
{
    return mDriver;
}

// 车辆是否被占用
bool Vehicle::isOccupied() const
{
    return mDriver != nullptr;
}

// 获取车辆损坏程度
float Vehicle::getDamage() const
{
    return mDamage;
}

// 增加车辆损坏
void Vehicle::addDamage(float amount)
{
    mDamage += amount;
    
    // 限制损坏程度在0-100之间
    if (mDamage < 0.f)
        mDamage = 0.f;
    else if (mDamage > 100.f)
        mDamage = 100.f;
    
    // 损坏程度影响最大速度和加速度
    float damageFactor = 1.f - mDamage / 100.f;
    mMaxSpeed *= damageFactor;
    mAcceleration *= damageFactor;
}