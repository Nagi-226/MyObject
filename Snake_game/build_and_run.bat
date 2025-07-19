@echo off
echo 正在构建贪吃蛇游戏...

:: 检查是否安装了Visual Studio
where cl >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo 未检测到Visual Studio编译器。
    echo 请确保已安装Visual Studio并在命令行中运行了vcvarsall.bat
    echo 或者使用开发者命令提示符运行此脚本。
    pause
    exit /b 1
)

:: 创建build目录
if not exist build mkdir build
cd build

:: 使用cl编译
echo 正在编译...
cl /EHsc /W4 /std:c++14 /Fe:snake_game.exe ..\main.cpp

if %ERRORLEVEL% neq 0 (
    echo 编译失败！
    cd ..
    pause
    exit /b 1
)

echo 编译成功！

:: 运行游戏
echo 正在启动游戏...
snake_game.exe

:: 返回上级目录
cd ..

pause