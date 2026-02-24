#include "Game.h"
#include <iostream>

int main()
{
    try
    {
        // 创建游戏实例
        Game game;

        // 初始化游戏
        game.initialize();

        // 运行游戏主循环
        game.run();

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred!" << std::endl;
        return 1;
    }
}