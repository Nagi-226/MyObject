cmake_minimum_required(VERSION 3.10)
project(GTAmini VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置SFML路径 - 用户需要修改这个路径指向他们的SFML安装目录
set(SFML_DIR "C:/SFML/lib/cmake/SFML" CACHE PATH "SFML root directory")

# 查找SFML包
find_package(SFML 2.5 COMPONENTS graphics audio window system)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接SFML库
if(SFML_FOUND)
    target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-window sfml-system)
    message(STATUS "SFML found, linking libraries")
else()
    message(WARNING "SFML not found. Please install SFML or set SFML_DIR to the correct path.")
    message(STATUS "You can download SFML from: https://www.sfml-dev.org/download.php")
    message(STATUS "After installing, set SFML_DIR to the path containing SFMLConfig.cmake")
    message(STATUS "For example: -DSFML_DIR=C:/SFML/lib/cmake/SFML")
endif()

# 复制资源文件到构建目录
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})