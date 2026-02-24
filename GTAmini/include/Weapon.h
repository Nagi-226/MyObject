#ifndef WEAPON_H
#define WEAPON_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

#include "Projectile.h"

// 武器类型枚举
enum class WeaponType {
    PISTOL,
    SHOTGUN,
    MACHINE_GUN,
    ROCKET_LAUNCHER
};

class Weapon {
public:
    // 构造函数和析构函数
    Weapon(WeaponType type);
    ~Weapon();

    // 更新武器状态
    void update(sf::Time deltaTime);
    
    // 渲染武器
    void render(sf::RenderWindow& window);

    // 武器行为
    void fire(sf::Vector2f position, sf::Vector2f direction);
    void reload();
    
    // 获取武器类型
    WeaponType getType() const;
    
    // 获取武器弹药
    int getAmmo() const;
    
    // 设置武器弹药
    void setAmmo(int ammo);
    
    // 获取武器弹夹容量
    int getMagazineSize() const;
    
    // 获取武器射速
    float getFireRate() const;
    
    // 获取武器伤害
    int getDamage() const;
    
    // 获取武器射程
    float getRange() const;
    
    // 获取武器精度
    float getAccuracy() const;
    
    // 获取武器投射物
    const std::vector<Projectile>& getProjectiles() const;

private:
    // 武器类型
    WeaponType mType;
    
    // 武器精灵
    sf::Sprite mSprite;
    
    // 武器纹理
    sf::Texture mTexture;
    
    // 武器射击音效
    sf::Sound mFireSound;
    
    // 武器射击音效缓冲区
    sf::SoundBuffer mFireSoundBuffer;
    
    // 武器弹药
    int mAmmo;
    
    // 武器弹夹容量
    int mMagazineSize;
    
    // 武器射速 (每秒发射次数)
    float mFireRate;
    
    // 武器伤害
    int mDamage;
    
    // 武器射程
    float mRange;
    
    // 武器精度 (0-1)
    float mAccuracy;
    
    // 武器冷却时间
    float mCooldown;
    
    // 武器投射物
    std::vector<Projectile> mProjectiles;
};

#endif // WEAPON_H