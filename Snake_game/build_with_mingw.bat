@echo off
echo 正在使用MinGW构建贪吃蛇游戏...

:: 检查是否安装了MinGW
where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo 未检测到MinGW的g++编译器。
    echo 请确保已安装MinGW并将其bin目录添加到PATH环境变量中。
    pause
    exit /b 1
)

:: 创建build目录
if not exist build_mingw mkdir build_mingw

:: 使用g++编译
echo 正在编译...
g++ -std=c++11 -Wall -o build_mingw\snake_game.exe main.cpp

if %ERRORLEVEL% neq 0 (
    echo 编译失败！
    pause
    exit /b 1
)

echo 编译成功！

:: 运行游戏
echo 正在启动游戏...
build_mingw\snake_game.exe

pause