# GTAmini - 俯视视角GTA风格游戏

这是一个使用C++和SFML库开发的类似1997年初代GTA的俯视视角游戏。游戏具备城市环境、车辆驾驶和射击元素。

## 安装与编译

### 依赖项
- C++17兼容的编译器 (GCC 7+, Visual Studio 2017+, Clang 5+)
- SFML 2.5.1
- CMake 3.10+

### 在Windows上编译
1. 安装SFML 2.5.1
   - 从 [SFML官网](https://www.sfml-dev.org/download.php) 下载并安装
   - 或使用vcpkg: `vcpkg install sfml:x64-windows`
2. 克隆仓库: `git clone https://github.com/yourusername/GTAmini.git`
3. 创建构建目录: `mkdir build && cd build`
4. 配置CMake (指定SFML路径): 
   - `cmake .. -DSFML_DIR=C:/path/to/SFML/lib/cmake/SFML`
5. 编译: `cmake --build . --config Release`

### 在Linux上编译
1. 安装依赖: `sudo apt install libsfml-dev cmake g++`
2. 克隆仓库: `git clone https://github.com/yourusername/GTAmini.git`
3. 创建构建目录: `mkdir build && cd build`
4. 配置并编译: `cmake .. && make`

### 在macOS上编译
1. 安装依赖: `brew install sfml cmake`
2. 克隆仓库: `git clone https://github.com/yourusername/GTAmini.git`
3. 创建构建目录: `mkdir build && cd build`
4. 配置并编译: `cmake .. && make`

## 功能特点

- 俯视视角的开放世界城市环境
- 玩家角色步行和驾驶车辆
- 射击系统和简单AI
- 基本任务系统
- 碰撞检测和物理系统

## 技术栈

- C++17
- SFML 2.5.1 (图形、音频、输入处理)
- 自定义游戏引擎

## 运行游戏

编译完成后，运行可执行文件即可启动游戏：
- Windows: `build\Release\GTAmini.exe`
- Linux/macOS: `build/GTAmini`

## 游戏控制

### 玩家控制
- WASD: 移动角色
- 鼠标左键: 射击
- R: 装弹
- 1-4: 切换武器
- E: 进入/离开车辆
- ESC: 暂停游戏

### 车辆控制
- W: 加速
- S: 刹车/倒车
- A/D: 左/右转向
- 空格: 手刹

## 开发计划

1. 基础架构设置 ✓
2. 地图和城市环境 ✓
3. 玩家控制系统 ✓
4. 车辆系统 ✓
5. 射击和战斗系统 ✓
6. AI和NPC ✓
7. 任务系统 (待实现)
8. 优化和完善 (进行中)

## 项目结构

```
├── assets/             # 游戏资源文件
│   ├── fonts/          # 字体文件
│   ├── sounds/         # 音效文件
│   └── textures/       # 纹理和图像文件
├── include/            # 头文件
│   ├── Entity.h        # 实体基类
│   ├── Game.h          # 游戏核心类
│   ├── Map.h           # 地图管理
│   ├── NPC.h           # NPC类
│   ├── Player.h        # 玩家类
│   ├── Projectile.h    # 投射物类
│   ├── Vehicle.h       # 车辆类
│   └── Weapon.h        # 武器类
├── src/                # 源文件
│   ├── Entity.cpp
│   ├── Game.cpp
│   ├── main.cpp        # 程序入口
│   ├── Map.cpp
│   ├── NPC.cpp
│   ├── Player.cpp
│   ├── Projectile.cpp
│   ├── Vehicle.cpp
│   └── Weapon.cpp
├── CMakeLists.txt      # CMake构建配置
└── README.md           # 项目说明
```

## 贡献指南

欢迎贡献代码、报告问题或提出改进建议！

1. Fork项目
2. 创建特性分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add some amazing feature'`)
4. 推送到分支 (`git push origin feature/amazing-feature`)
5. 创建Pull Request

## 许可证

本项目采用MIT许可证 - 详情请参阅 [LICENSE](LICENSE) 文件