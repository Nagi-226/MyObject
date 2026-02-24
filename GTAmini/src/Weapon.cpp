#include "Weapon.h"
#include <iostream>
#include <cmath>

// 构造函数
Weapon::Weapon(WeaponType type)
    : mType(type)
    , mCooldown(0.f)
{
    // 根据武器类型设置属性
    switch (type)
    {
        case WeaponType::PISTOL:
            mAmmo = 12;
            mMagazineSize = 12;
            mFireRate = 2.f; // 每秒发射次数
            mDamage = 20;
            mRange = 300.f;
            mAccuracy = 0.9f;
            
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/pistol.png"))
            {
                std::cerr << "Failed to load pistol texture!" << std::endl;
            }
            
            // 加载射击音效
            if (!mFireSoundBuffer.loadFromFile("assets/sounds/pistol_shot.wav"))
            {
                std::cerr << "Failed to load pistol shot sound!" << std::endl;
            }
            break;
            
        case WeaponType::SHOTGUN:
            mAmmo = 8;
            mMagazineSize = 8;
            mFireRate = 1.f;
            mDamage = 15; // 每个弹丸的伤害
            mRange = 200.f;
            mAccuracy = 0.7f;
            
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/shotgun.png"))
            {
                std::cerr << "Failed to load shotgun texture!" << std::endl;
            }
            
            // 加载射击音效
            if (!mFireSoundBuffer.loadFromFile("assets/sounds/shotgun_shot.wav"))
            {
                std::cerr << "Failed to load shotgun shot sound!" << std::endl;
            }
            break;
            
        case WeaponType::MACHINE_GUN:
            mAmmo = 30;
            mMagazineSize = 30;
            mFireRate = 10.f;
            mDamage = 10;
            mRange = 400.f;
            mAccuracy = 0.8f;
            
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/machine_gun.png"))
            {
                std::cerr << "Failed to load machine gun texture!" << std::endl;
            }
            
            // 加载射击音效
            if (!mFireSoundBuffer.loadFromFile("assets/sounds/machine_gun_shot.wav"))
            {
                std::cerr << "Failed to load machine gun shot sound!" << std::endl;
            }
            break;
            
        case WeaponType::ROCKET_LAUNCHER:
            mAmmo = 4;
            mMagazineSize = 4;
            mFireRate = 0.5f;
            mDamage = 100;
            mRange = 500.f;
            mAccuracy = 0.95f;
            
            // 加载纹理
            if (!mTexture.loadFromFile("assets/textures/rocket_launcher.png"))
            {
                std::cerr << "Failed to load rocket launcher texture!" << std::endl;
            }
            
            // 加载射击音效
            if (!mFireSoundBuffer.loadFromFile("assets/sounds/rocket_launcher_shot.wav"))
            {
                std::cerr << "Failed to load rocket launcher shot sound!" << std::endl;
            }
            break;
    }
    
    // 设置精灵
    mSprite.setTexture(mTexture);
    mSprite.setOrigin(mTexture.getSize().x / 2.f, mTexture.getSize().y / 2.f);
    
    // 设置射击音效
    mFireSound.setBuffer(mFireSoundBuffer);
}

// 析构函数
Weapon::~Weapon()
{
}

// 更新武器状态
void Weapon::update(sf::Time deltaTime)
{
    // 更新冷却时间
    if (mCooldown > 0.f)
    {
        mCooldown -= deltaTime.asSeconds();
        if (mCooldown < 0.f)
            mCooldown = 0.f;
    }
    
    // 更新投射物
    for (auto it = mProjectiles.begin(); it != mProjectiles.end();)
    {
        it->update(deltaTime);
        
        // 如果投射物不再活跃，移除它
        if (!it->isActive())
        {
            it = mProjectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// 渲染武器
void Weapon::render(sf::RenderWindow& window)
{
    // 渲染投射物
    for (auto& projectile : mProjectiles)
    {
        projectile.render(window);
    }
    
    // 渲染武器精灵
    // 注意：在实际实现中，武器应该跟随玩家位置和方向
    // window.draw(mSprite);
}

// 武器射击
void Weapon::fire(sf::Vector2f position, sf::Vector2f direction)
{
    // 检查冷却时间和弹药
    if (mCooldown <= 0.f && mAmmo > 0)
    {
        // 设置冷却时间
        mCooldown = 1.f / mFireRate;
        
        // 减少弹药
        --mAmmo;
        
        // 播放射击音效
        mFireSound.play();
        
        // 根据武器类型创建投射物
        switch (mType)
        {
            case WeaponType::PISTOL:
            {
                // 添加精度偏移
                sf::Vector2f spreadDirection = applySpread(direction, mAccuracy);
                
                // 创建子弹
                mProjectiles.emplace_back(ProjectileType::BULLET, position, spreadDirection, 800.f, mDamage, mRange);
                break;
            }
                
            case WeaponType::SHOTGUN:
            {
                // 散弹枪发射多个弹丸
                const int pelletCount = 8;
                for (int i = 0; i < pelletCount; ++i)
                {
                    // 添加较大的精度偏移
                    sf::Vector2f spreadDirection = applySpread(direction, mAccuracy * 0.7f);
                    
                    // 创建弹丸
                    mProjectiles.emplace_back(ProjectileType::SHOTGUN_PELLET, position, spreadDirection, 600.f, mDamage, mRange);
                }
                break;
            }
                
            case WeaponType::MACHINE_GUN:
            {
                // 添加精度偏移
                sf::Vector2f spreadDirection = applySpread(direction, mAccuracy);
                
                // 创建子弹
                mProjectiles.emplace_back(ProjectileType::BULLET, position, spreadDirection, 900.f, mDamage, mRange);
                break;
            }
                
            case WeaponType::ROCKET_LAUNCHER:
            {
                // 火箭发射器精度较高
                sf::Vector2f spreadDirection = applySpread(direction, mAccuracy);
                
                // 创建火箭
                mProjectiles.emplace_back(ProjectileType::ROCKET, position, spreadDirection, 500.f, mDamage, mRange);
                break;
            }
        }
    }
}

// 武器装弹
void Weapon::reload()
{
    mAmmo = mMagazineSize;
}

// 获取武器类型
WeaponType Weapon::getType() const
{
    return mType;
}

// 获取武器弹药
int Weapon::getAmmo() const
{
    return mAmmo;
}

// 设置武器弹药
void Weapon::setAmmo(int ammo)
{
    mAmmo = ammo;
    if (mAmmo > mMagazineSize)
        mAmmo = mMagazineSize;
    else if (mAmmo < 0)
        mAmmo = 0;
}

// 获取武器弹夹容量
int Weapon::getMagazineSize() const
{
    return mMagazineSize;
}

// 获取武器射速
float Weapon::getFireRate() const
{
    return mFireRate;
}

// 获取武器伤害
int Weapon::getDamage() const
{
    return mDamage;
}

// 获取武器射程
float Weapon::getRange() const
{
    return mRange;
}

// 获取武器精度
float Weapon::getAccuracy() const
{
    return mAccuracy;
}

// 获取武器投射物
const std::vector<Projectile>& Weapon::getProjectiles() const
{
    return mProjectiles;
}

// 应用精度偏移
sf::Vector2f Weapon::applySpread(sf::Vector2f direction, float accuracy)
{
    // 计算最大偏移角度（弧度）
    float maxSpreadRadians = (1.f - accuracy) * 0.5f;
    
    // 生成随机偏移角度
    float spreadRadians = (static_cast<float>(rand()) / RAND_MAX * 2.f - 1.f) * maxSpreadRadians;
    
    // 计算原始方向的角度
    float directionRadians = std::atan2(direction.y, direction.x);
    
    // 应用偏移
    float newDirectionRadians = directionRadians + spreadRadians;
    
    // 计算新方向向量
    sf::Vector2f newDirection(std::cos(newDirectionRadians), std::sin(newDirectionRadians));
    
    // 归一化方向向量
    float length = std::sqrt(newDirection.x * newDirection.x + newDirection.y * newDirection.y);
    if (length != 0.f)
        newDirection /= length;
    
    return newDirection;
}