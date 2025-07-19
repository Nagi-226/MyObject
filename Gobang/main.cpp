#include <iostream>
#include <conio.h>
#include <windows.h>
#include "game.h"

int main() {
    SetConsoleOutputCP(65001); // 设置控制台输出为UTF-8编码
    
    Game game;
    game.start();
    
    return 0;
}