@echo off
echo 正在编译五子棋游戏...

:: 检查是否有C++编译器
where g++ >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo 使用g++编译...
    g++ -std=c++11 Gobang.cpp -o Gobang.exe
    goto check_result
)

where cl >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo 使用Visual C++编译...
    cl /EHsc /std:c++14 Gobang.cpp /link user32.lib gdi32.lib
    goto check_result
)

echo 未检测到C++编译器。
echo 请安装MinGW或Visual Studio，或使用在线编译器编译此代码。
echo.
echo 您可以下载以下编译器之一：
echo 1. MinGW-w64: https://www.mingw-w64.org/downloads/
echo 2. Visual Studio Community: https://visualstudio.microsoft.com/vs/community/
echo.
echo 或者使用在线编译器：
echo - Compiler Explorer: https://godbolt.org/
echo - OnlineGDB: https://www.onlinegdb.com/
echo.
echo 如果您已安装编译器但仍看到此消息，请确保编译器已添加到系统PATH中。
goto end

:check_result
if %ERRORLEVEL% EQU 0 (
    echo 编译成功！
    echo 可执行文件已生成：Gobang.exe
) else (
    echo 编译失败，请检查错误信息。
)

:end
pause