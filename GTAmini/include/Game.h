#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <string>

#include "Player.h"
#include "Map.h"
#include "Vehicle.h"
#include "Weapon.h"
#include "NPC.h"

class Game {
public:
    // 构造函数和析构函数
    Game();
    ~Game();

    // 游戏主循环
    void run();

private:
    // 初始化游戏
    void initialize();
    
    // 处理输入
    void processInput();
    
    // 更新游戏状态
    void update(sf::Time deltaTime);
    
    // 渲染游戏
    void render();

    // 加载资源
    void loadResources();

    // SFML窗口
    sf::RenderWindow mWindow;
    
    // 游戏时钟
    sf::Clock mClock;
    
    // 游戏组件
    std::unique_ptr<Player> mPlayer;
    std::unique_ptr<Map> mMap;
    std::vector<std::unique_ptr<Vehicle>> mVehicles;
    std::vector<std::unique_ptr<NPC>> mNPCs;
    
    // 游戏状态
    bool mIsRunning;
    
    // 游戏视图
    sf::View mGameView;
};

#endif // GAME_H