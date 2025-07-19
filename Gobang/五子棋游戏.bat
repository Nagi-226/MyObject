@echo off
echo 正在启动五子棋游戏...

:: 检查是否存在可执行文件
if exist Gobang.exe (
    start Gobang.exe
) else (
    echo 未找到游戏可执行文件，正在尝试编译...
    call compile.bat
    
    if exist Gobang.exe (
        echo 编译成功，正在启动游戏...
        start Gobang.exe
    ) else (
        echo 无法启动游戏，请先编译游戏。
        pause
    )
)