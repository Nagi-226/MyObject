cmake_minimum_required(VERSION 3.16)
project(SuperCaculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 使用 Qt Widgets
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Expression.cpp
    src/Symbolic.cpp
    src/Numerics.cpp
)

set(HEADERS
    include/MainWindow.h
    include/Expression.h
    include/Symbolic.h
    include/Numerics.h
)

set(UI_FORMS
    ui/MainWindow.ui
)

qt_wrap_ui(UI_SOURCES ${UI_FORMS})

add_executable(SuperCaculator
    ${SOURCES}
    ${HEADERS}
    ${UI_SOURCES}
)

target_include_directories(SuperCaculator PRIVATE include)

target_link_libraries(SuperCaculator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)




