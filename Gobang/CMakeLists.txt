cmake_minimum_required(VERSION 3.10)

project(Gobang VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(Gobang 
    main.cpp
    game.cpp
    board.cpp
    ai.cpp
)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置生成的可执行文件名
set_target_properties(Gobang PROPERTIES OUTPUT_NAME "Gobang")

# 如果是Windows系统，设置为控制台应用程序
if(WIN32)
    set_target_properties(Gobang PROPERTIES WIN32_EXECUTABLE FALSE)
    # 添加Windows特定的链接选项
    target_link_libraries(Gobang PRIVATE user32 gdi32)
endif()