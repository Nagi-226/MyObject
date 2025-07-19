@echo off
echo 正在构建五子棋游戏...

:: 创建构建目录
if not exist build mkdir build
cd build

:: 运行CMake配置
cmake -G "MinGW Makefiles" ..

:: 编译项目
cmake --build . --config Release

:: 复制可执行文件到项目根目录
if exist bin\Gobang.exe (
    copy bin\Gobang.exe ..\
    echo 构建成功！可执行文件已生成：Gobang.exe
) else (
    echo 构建失败，请检查错误信息。
)

:: 返回项目根目录
cd ..

pause