#include "Game.h"
#include <iostream>

// 构造函数
Game::Game()
    : mWindow(sf::VideoMode(800, 600), "GTAmini - 俯视视角GTA风格游戏", sf::Style::Close)
    , mIsRunning(false)
{
    // 初始化游戏
    initialize();
}

// 析构函数
Game::~Game()
{
}

// 初始化游戏
void Game::initialize()
{
    // 设置窗口帧率限制
    mWindow.setFramerateLimit(60);
    
    // 加载资源
    loadResources();
    
    // 创建地图
    mMap = std::make_unique<Map>();
    mMap->generateRandomCity(50, 50);
    
    // 创建玩家
    mPlayer = std::make_unique<Player>();
    mPlayer->setPosition(sf::Vector2f(400.f, 300.f));
    
    // 设置游戏视图
    mGameView.setSize(800.f, 600.f);
    mGameView.setCenter(mPlayer->getPosition());
    
    // 创建一些车辆
    mVehicles.push_back(std::make_unique<Vehicle>(VehicleType::SEDAN, sf::Vector2f(300.f, 300.f)));
    mVehicles.push_back(std::make_unique<Vehicle>(VehicleType::SPORTS_CAR, sf::Vector2f(500.f, 400.f)));
    mVehicles.push_back(std::make_unique<Vehicle>(VehicleType::POLICE_CAR, sf::Vector2f(200.f, 500.f)));
    
    // 创建一些NPC
    mNPCs.push_back(std::make_unique<NPC>(NPCType::CIVILIAN, sf::Vector2f(350.f, 350.f)));
    mNPCs.push_back(std::make_unique<NPC>(NPCType::POLICE, sf::Vector2f(450.f, 450.f)));
    mNPCs.push_back(std::make_unique<NPC>(NPCType::GANGSTER, sf::Vector2f(550.f, 250.f)));
    
    // 设置游戏运行状态
    mIsRunning = true;
}

// 加载资源
void Game::loadResources()
{
    // 这里将加载游戏所需的所有资源
    // 如纹理、音效、字体等
    // 在实际实现中，应该使用资源管理器来管理这些资源
}

// 游戏主循环
void Game::run()
{
    // 游戏循环
    while (mIsRunning && mWindow.isOpen())
    {
        // 计算帧间时间
        sf::Time deltaTime = mClock.restart();
        
        // 处理输入
        processInput();
        
        // 更新游戏状态
        update(deltaTime);
        
        // 渲染游戏
        render();
    }
}

// 处理输入
void Game::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        // 关闭窗口
        if (event.type == sf::Event::Closed)
        {
            mWindow.close();
            mIsRunning = false;
        }
        
        // 处理玩家输入
        mPlayer->handleInput(event);
    }
    
    // 处理持续按键输入
    sf::Vector2f direction(0.f, 0.f);
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        direction.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        direction.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        direction.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        direction.x += 1.f;
    
    // 归一化方向向量
    if (direction.x != 0.f || direction.y != 0.f)
    {
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;
    }
    
    // 根据玩家状态处理输入
    if (mPlayer->getState() == Player::State::WALKING)
    {
        mPlayer->walk(direction);
    }
    else if (mPlayer->getState() == Player::State::DRIVING)
    {
        // 处理驾驶输入
        // 这里需要获取玩家当前驾驶的车辆并控制它
    }
    
    // 处理射击输入
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
        mPlayer->getState() != Player::State::DRIVING)
    {
        mPlayer->shoot();
    }
}

// 更新游戏状态
void Game::update(sf::Time deltaTime)
{
    // 更新玩家
    mPlayer->update(deltaTime);
    
    // 更新地图
    mMap->update(deltaTime);
    
    // 更新车辆
    for (auto& vehicle : mVehicles)
    {
        vehicle->update(deltaTime);
    }
    
    // 更新NPC
    for (auto& npc : mNPCs)
    {
        npc->update(deltaTime);
    }
    
    // 更新游戏视图，跟随玩家
    mGameView.setCenter(mPlayer->getPosition());
}

// 渲染游戏
void Game::render()
{
    // 清空窗口
    mWindow.clear(sf::Color(100, 100, 100));
    
    // 设置视图
    mWindow.setView(mGameView);
    
    // 渲染地图
    mMap->render(mWindow);
    
    // 渲染车辆
    for (auto& vehicle : mVehicles)
    {
        vehicle->render(mWindow);
    }
    
    // 渲染NPC
    for (auto& npc : mNPCs)
    {
        npc->render(mWindow);
    }
    
    // 渲染玩家
    mPlayer->render(mWindow);
    
    // 显示窗口内容
    mWindow.display();
}