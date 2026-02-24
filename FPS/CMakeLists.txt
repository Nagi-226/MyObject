cmake_minimum_required(VERSION 3.10)
project(FPS LANGUAGES CXX)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 可执行文件：Windows 用 WIN32 子系统，否则控制台
set(FPS_SOURCES FPS.cpp)
if(WIN32)
    add_executable(FPS WIN32 ${FPS_SOURCES})
    target_compile_definitions(FPS PRIVATE UNICODE _UNICODE)
    target_link_libraries(FPS PRIVATE winmm)
else()
    add_executable(FPS ${FPS_SOURCES})
endif()

# 头文件目录（当前目录，用于 olcConsoleGameEngine.h）
target_include_directories(FPS PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 输出到 FPS 子目录，便于在项目根目录统一 build
set_target_properties(FPS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
