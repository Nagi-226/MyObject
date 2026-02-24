#include "NPC.h"
#include "Vehicle.h"
#include <iostream>
#include <cmath>
#include <random>

// 构造函数
NPC::NPC(NPCType type, sf::Vector2f position)
    : mType(type)
    , mState(NPCState::IDLE)
    , mSpeed(100.f)
    , mDirection(0.f, 0.f)
    , mCurrentVehicle(nullptr)
    , mHealth(100)
    , mTarget(nullptr)
    , mThreat(nullptr)
    , mDecisionTimer(0.f)
    , mDecisionInterval(2.f)
{
    // 设置初始位置
    mPosition = position;
    
    // 根据NPC类型设置属性
    switch (type)
    {
        case NPCType::CIVILIAN:
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/civilian.png"))
            {
                std::cerr << "Failed to load civilian texture!" << std::endl;
            }
            
            // 平民没有武器
            mWeapon = nullptr;
            break;
            
        case NPCType::POLICE:
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/police.png"))
            {
                std::cerr << "Failed to load police texture!" << std::endl;
            }
            
            // 警察配备手枪
            mWeapon = std::make_unique<Weapon>(WeaponType::PISTOL);
            break;
            
        case NPCType::GANGSTER:
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/gangster.png"))
            {
                std::cerr << "Failed to load gangster texture!" << std::endl;
            }
            
            // 匪徒配备冲锋枪
            mWeapon = std::make_unique<Weapon>(WeaponType::MACHINE_GUN);
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
NPC::~NPC()
{
}

// 更新NPC状态
void NPC::update(sf::Time deltaTime)
{
    // 更新AI行为
    updateAI(deltaTime);
    
    // 根据NPC状态更新
    switch (mState)
    {
        case NPCState::IDLE:
            // 闲置状态下不移动
            break;
            
        case NPCState::WALKING:
            // 更新位置
            mPosition += mDirection * mSpeed * deltaTime.asSeconds();
            break;
            
        case NPCState::RUNNING:
            // 更新位置（跑步速度更快）
            mPosition += mDirection * (mSpeed * 1.5f) * deltaTime.asSeconds();
            break;
            
        case NPCState::DRIVING:
            // 如果NPC在驾驶车辆，位置跟随车辆
            if (mCurrentVehicle)
            {
                mPosition = mCurrentVehicle->getPosition();
            }
            break;
            
        case NPCState::ATTACKING:
            // 攻击状态下，NPC会朝向目标但不移动
            if (mTarget)
            {
                // 计算朝向目标的方向
                sf::Vector2f targetDirection = mTarget->getPosition() - mPosition;
                float length = std::sqrt(targetDirection.x * targetDirection.x + targetDirection.y * targetDirection.y);
                if (length > 0.f)
                {
                    targetDirection /= length;
                    mDirection = targetDirection;
                }
                
                // 使用武器攻击目标
                if (mWeapon)
                {
                    mWeapon->fire(mPosition, mDirection);
                }
            }
            break;
            
        case NPCState::FLEEING:
            // 逃跑状态下，NPC会远离威胁
            if (mThreat)
            {
                // 计算远离威胁的方向
                sf::Vector2f fleeDirection = mPosition - mThreat->getPosition();
                float length = std::sqrt(fleeDirection.x * fleeDirection.x + fleeDirection.y * fleeDirection.y);
                if (length > 0.f)
                {
                    fleeDirection /= length;
                    mDirection = fleeDirection;
                }
                
                // 更新位置（逃跑速度更快）
                mPosition += mDirection * (mSpeed * 1.8f) * deltaTime.asSeconds();
            }
            break;
    }
    
    // 更新精灵位置和旋转
    mSprite.setPosition(mPosition);
    
    // 如果NPC有方向，则设置旋转角度
    if (mDirection.x != 0.f || mDirection.y != 0.f)
    {
        float angle = std::atan2(mDirection.y, mDirection.x) * 180.f / 3.14159f;
        mSprite.setRotation(angle + 90.f); // +90度是因为精灵默认朝上
        mRotation = angle + 90.f;
    }
    
    // 更新碰撞边界位置
    mBounds.left = mPosition.x - mBounds.width / 2.f;
    mBounds.top = mPosition.y - mBounds.height / 2.f;
    
    // 更新武器
    if (mWeapon)
    {
        mWeapon->update(deltaTime);
    }
}

// 更新AI行为
void NPC::updateAI(sf::Time deltaTime)
{
    // 更新决策计时器
    mDecisionTimer -= deltaTime.asSeconds();
    
    // 到达决策时间
    if (mDecisionTimer <= 0.f)
    {
        // 重置计时器
        mDecisionTimer = mDecisionInterval;
        
        // 创建随机数生成器
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, 100);
        std::uniform_real_distribution<float> angleDist(0.f, 360.f);
        
        // 根据NPC类型和当前状态做出决策
        switch (mType)
        {
            case NPCType::CIVILIAN:
                // 平民主要是闲逛或逃跑
                if (mThreat)
                {
                    // 如果有威胁，逃跑
                    setState(NPCState::FLEEING);
                }
                else
                {
                    // 随机决定是闲置还是行走
                    int decision = distr(gen);
                    if (decision < 30)
                    {
                        setState(NPCState::IDLE);
                    }
                    else
                    {
                        setState(NPCState::WALKING);
                        
                        // 随机方向
                        float angle = angleDist(gen) * 3.14159f / 180.f;
                        mDirection.x = std::cos(angle);
                        mDirection.y = std::sin(angle);
                    }
                }
                break;
                
            case NPCType::POLICE:
                // 警察会追捕目标或巡逻
                if (mTarget)
                {
                    // 如果有目标，攻击或追捕
                    float distanceToTarget = std::sqrt(
                        std::pow(mTarget->getPosition().x - mPosition.x, 2) +
                        std::pow(mTarget->getPosition().y - mPosition.y, 2));
                    
                    if (distanceToTarget < 200.f && mWeapon)
                    {
                        // 在射程内，攻击
                        setState(NPCState::ATTACKING);
                    }
                    else
                    {
                        // 追捕目标
                        setState(NPCState::RUNNING);
                        
                        // 计算朝向目标的方向
                        sf::Vector2f targetDirection = mTarget->getPosition() - mPosition;
                        float length = std::sqrt(targetDirection.x * targetDirection.x + targetDirection.y * targetDirection.y);
                        if (length > 0.f)
                        {
                            targetDirection /= length;
                            mDirection = targetDirection;
                        }
                    }
                }
                else
                {
                    // 随机巡逻
                    setState(NPCState::WALKING);
                    
                    // 随机方向
                    float angle = angleDist(gen) * 3.14159f / 180.f;
                    mDirection.x = std::cos(angle);
                    mDirection.y = std::sin(angle);
                }
                break;
                
            case NPCType::GANGSTER:
                // 匪徒会攻击目标或游荡
                if (mTarget)
                {
                    // 如果有目标，攻击
                    float distanceToTarget = std::sqrt(
                        std::pow(mTarget->getPosition().x - mPosition.x, 2) +
                        std::pow(mTarget->getPosition().y - mPosition.y, 2));
                    
                    if (distanceToTarget < 300.f && mWeapon)
                    {
                        // 在射程内，攻击
                        setState(NPCState::ATTACKING);
                    }
                    else
                    {
                        // 追捕目标
                        setState(NPCState::RUNNING);
                        
                        // 计算朝向目标的方向
                        sf::Vector2f targetDirection = mTarget->getPosition() - mPosition;
                        float length = std::sqrt(targetDirection.x * targetDirection.x + targetDirection.y * targetDirection.y);
                        if (length > 0.f)
                        {
                            targetDirection /= length;
                            mDirection = targetDirection;
                        }
                    }
                }
                else
                {
                    // 随机游荡或闲置
                    int decision = distr(gen);
                    if (decision < 40)
                    {
                        setState(NPCState::IDLE);
                    }
                    else
                    {
                        setState(NPCState::WALKING);
                        
                        // 随机方向
                        float angle = angleDist(gen) * 3.14159f / 180.f;
                        mDirection.x = std::cos(angle);
                        mDirection.y = std::sin(angle);
                    }
                }
                break;
        }
    }
}

// 渲染NPC
void NPC::render(sf::RenderWindow& window)
{
    // 根据NPC状态渲染
    switch (mState)
    {
        case NPCState::IDLE:
        case NPCState::WALKING:
        case NPCState::RUNNING:
        case NPCState::ATTACKING:
        case NPCState::FLEEING:
            // 渲染NPC精灵
            window.draw(mSprite);
            
            // 渲染武器
            if (mWeapon)
            {
                mWeapon->render(window);
            }
            break;
            
        case NPCState::DRIVING:
            // 驾驶状态下不渲染NPC，由车辆类负责渲染
            break;
    }
}

// NPC行走
void NPC::walk(sf::Vector2f direction)
{
    mDirection = direction;
    mState = NPCState::WALKING;
}

// NPC奔跑
void NPC::run(sf::Vector2f direction)
{
    mDirection = direction;
    mState = NPCState::RUNNING;
}

// NPC攻击
void NPC::attack(Entity* target)
{
    mTarget = target;
    mState = NPCState::ATTACKING;
}

// NPC逃跑
void NPC::flee(Entity* threat)
{
    mThreat = threat;
    mState = NPCState::FLEEING;
}

// NPC进入车辆
void NPC::enterVehicle(Vehicle* vehicle)
{
    if (vehicle && !vehicle->isOccupied())
    {
        mCurrentVehicle = vehicle;
        mState = NPCState::DRIVING;
        vehicle->setDriver(nullptr); // NPC不是Player类型，所以这里传nullptr
    }
}

// NPC离开车辆
void NPC::exitVehicle()
{
    if (mCurrentVehicle)
    {
        // 设置NPC位置在车辆旁边
        sf::Vector2f offset(30.f, 0.f); // 偏移量
        mPosition = mCurrentVehicle->getPosition() + offset;
        
        // 更新精灵位置
        mSprite.setPosition(mPosition);
        
        // 清除车辆驾驶员
        mCurrentVehicle->setDriver(nullptr);
        mCurrentVehicle = nullptr;
        
        // 更改状态为闲置
        mState = NPCState::IDLE;
    }
}

// 获取NPC类型
NPCType NPC::getType() const
{
    return mType;
}

// 获取NPC状态
NPCState NPC::getState() const
{
    return mState;
}

// 设置NPC状态
void NPC::setState(NPCState state)
{
    mState = state;
}