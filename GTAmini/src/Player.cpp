#include "Player.h"
#include "Vehicle.h"
#include <iostream>
#include <cmath>

// 构造函数
Player::Player()
    : mState(State::WALKING)
    , mSpeed(200.f)
    , mDirection(0.f, 0.f)
    , mCurrentVehicle(nullptr)
    , mHealth(100)
    , mArmor(0)
    , mMoney(0)
{
    // 加载玩家纹理
    if (!mTexture.loadFromFile("assets/textures/player.png"))
    {
        std::cerr << "Failed to load player texture!" << std::endl;
    }
    
    // 设置玩家精灵
    mSprite.setTexture(mTexture);
    mSprite.setOrigin(mTexture.getSize().x / 2.f, mTexture.getSize().y / 2.f);
    
    // 设置碰撞边界
    mBounds.width = mTexture.getSize().x * 0.8f;
    mBounds.height = mTexture.getSize().y * 0.8f;
    
    // 创建默认武器
    mCurrentWeapon = std::make_unique<Weapon>(WeaponType::PISTOL);
}

// 析构函数
Player::~Player()
{
}

// 更新玩家状态
void Player::update(sf::Time deltaTime)
{
    // 根据玩家状态更新
    switch (mState)
    {
        case State::WALKING:
            // 更新玩家位置
            mPosition += mDirection * mSpeed * deltaTime.asSeconds();
            
            // 更新精灵位置和旋转
            mSprite.setPosition(mPosition);
            
            // 如果玩家有方向，则设置旋转角度
            if (mDirection.x != 0.f || mDirection.y != 0.f)
            {
                float angle = std::atan2(mDirection.y, mDirection.x) * 180.f / 3.14159f;
                mSprite.setRotation(angle + 90.f); // +90度是因为精灵默认朝上
                mRotation = angle + 90.f;
            }
            break;
            
        case State::DRIVING:
            // 如果玩家在驾驶车辆，位置跟随车辆
            if (mCurrentVehicle)
            {
                mPosition = mCurrentVehicle->getPosition();
                mSprite.setPosition(mPosition);
            }
            break;
            
        case State::SHOOTING:
            // 射击状态下，玩家可以移动但速度降低
            mPosition += mDirection * (mSpeed * 0.5f) * deltaTime.asSeconds();
            mSprite.setPosition(mPosition);
            break;
    }
    
    // 更新碰撞边界位置
    mBounds.left = mPosition.x - mBounds.width / 2.f;
    mBounds.top = mPosition.y - mBounds.height / 2.f;
    
    // 更新武器
    if (mCurrentWeapon)
    {
        mCurrentWeapon->update(deltaTime);
    }
}

// 处理输入
void Player::handleInput(const sf::Event& event)
{
    // 处理按键按下事件
    if (event.type == sf::Event::KeyPressed)
    {
        // 进入/离开车辆
        if (event.key.code == sf::Keyboard::E)
        {
            if (mState == State::WALKING)
            {
                // 尝试进入附近的车辆
                // 这里需要检测附近的车辆并进入
                // 在实际实现中，应该由Game类来处理这个逻辑
            }
            else if (mState == State::DRIVING)
            {
                // 离开当前车辆
                exitVehicle();
            }
        }
        
        // 切换武器
        if (event.key.code == sf::Keyboard::Q && mState != State::DRIVING)
        {
            // 切换武器逻辑
            // 在实际实现中，应该有一个武器列表并循环切换
        }
    }
}

// 渲染玩家
void Player::render(sf::RenderWindow& window)
{
    // 根据玩家状态渲染
    switch (mState)
    {
        case State::WALKING:
        case State::SHOOTING:
            // 渲染玩家精灵
            window.draw(mSprite);
            
            // 渲染武器
            if (mCurrentWeapon)
            {
                mCurrentWeapon->render(window);
            }
            break;
            
        case State::DRIVING:
            // 驾驶状态下不渲染玩家，由车辆类负责渲染
            break;
    }
}

// 玩家行走
void Player::walk(sf::Vector2f direction)
{
    // 只有在步行状态下才能行走
    if (mState == State::WALKING || mState == State::SHOOTING)
    {
        mDirection = direction;
    }
}

// 玩家射击
void Player::shoot()
{
    // 只有在步行状态下才能射击
    if (mState == State::WALKING)
    {
        mState = State::SHOOTING;
    }
    
    // 使用当前武器射击
    if (mCurrentWeapon)
    {
        // 计算射击方向（朝向鼠标位置）
        // 在实际实现中，需要获取鼠标在世界坐标中的位置
        sf::Vector2f direction(std::cos((mRotation - 90.f) * 3.14159f / 180.f),
                              std::sin((mRotation - 90.f) * 3.14159f / 180.f));
        
        mCurrentWeapon->fire(mPosition, direction);
    }
}

// 进入车辆
void Player::enterVehicle(Vehicle* vehicle)
{
    if (vehicle && !vehicle->isOccupied())
    {
        mCurrentVehicle = vehicle;
        mState = State::DRIVING;
        vehicle->setDriver(this);
    }
}

// 离开车辆
void Player::exitVehicle()
{
    if (mCurrentVehicle)
    {
        // 设置玩家位置在车辆旁边
        sf::Vector2f offset(30.f, 0.f); // 偏移量
        mPosition = mCurrentVehicle->getPosition() + offset;
        
        // 更新精灵位置
        mSprite.setPosition(mPosition);
        
        // 清除车辆驾驶员
        mCurrentVehicle->setDriver(nullptr);
        mCurrentVehicle = nullptr;
        
        // 更改状态为步行
        mState = State::WALKING;
    }
}

// 获取玩家状态
Player::State Player::getState() const
{
    return mState;
}

// 获取玩家位置
sf::Vector2f Player::getPosition() const
{
    return mPosition;
}

// 设置玩家位置
void Player::setPosition(sf::Vector2f position)
{
    mPosition = position;
    mSprite.setPosition(position);
    
    // 更新碰撞边界位置
    mBounds.left = position.x - mBounds.width / 2.f;
    mBounds.top = position.y - mBounds.height / 2.f;
}