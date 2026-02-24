# FPS 简易第一人称游戏 - 运行与调试

## 方式一：VS Code 内直接运行/调试（推荐）

1. **首次使用**：在 VS Code 中按 `Ctrl+Shift+B` 编译，或按 `F5` 启动调试（会自动先编译）。
2. **调试**：按 `F5` 选择「调试 FPS 游戏」即可运行并断点调试。
3. **可执行文件位置**：
   - 使用 Ninja/MinGW 等时：`build/FPS/FPS.exe`
   - 使用 Visual Studio 生成器时：`build/FPS/Debug/FPS.exe`（请选「调试 FPS 游戏 (MSVC Debug 目录)」）

## 方式二：命令行 CMake 编译

在项目**根目录**（即 `MyObject` 或包含 `FPS` 文件夹的目录）执行：

```bash
# 配置
cmake -S FPS -B build/FPS -DCMAKE_BUILD_TYPE=Debug

# 编译
cmake --build build/FPS
```

运行：

```bash
# 若为 Ninja/MinGW
build\FPS\FPS.exe

# 若为 Visual Studio 生成器
build\FPS\Debug\FPS.exe
```

## 操作说明

- **W/A/S/D**：移动与转向  
- **空格** 或 **鼠标左键**：射击  
- **R**：换弹  
