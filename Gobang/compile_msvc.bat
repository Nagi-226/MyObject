@echo off
echo 正在编译五子棋游戏...

:: 检查是否有Visual Studio环境
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo 未检测到Visual Studio编译器。
    echo 请安装Visual Studio或确保已打开开发者命令提示符。
    echo.
    echo 您可以下载Visual Studio Community版本（免费）：
    echo https://visualstudio.microsoft.com/vs/community/
    pause
    exit /b 1
)

:: 创建输出目录
if not exist build mkdir build

:: 编译所有源文件
echo 正在编译源文件...
cl /EHsc /std:c++14 /Fe:Gobang.exe main.cpp game.cpp board.cpp ai.cpp /link user32.lib gdi32.lib

:: 检查编译结果
if %ERRORLEVEL% EQU 0 (
    echo 编译成功！
    echo 可执行文件已生成：Gobang.exe
) else (
    echo 编译失败，请检查错误信息。
)

pause