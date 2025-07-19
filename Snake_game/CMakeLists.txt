cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(snake_game main.cpp)

# Windows特定设置
if(WIN32)
    target_link_libraries(snake_game winmm)
    # 设置控制台应用程序
    set_target_properties(snake_game PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

# 输出目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)