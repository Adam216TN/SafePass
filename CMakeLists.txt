cmake_minimum_required(VERSION 3.16)
project(SafePass VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

# On indique où sont les headers (.h)
include_directories(include)

# On liste les fichiers sources
add_executable(SafePass 
    src/main.cpp
    src/mainwindow.cpp
    src/vaultwindow.cpp
    src/registerwindow.cpp
    src/databasemanager.cpp
    include/mainwindow.h
    include/vaultwindow.h
    include/registerwindow.h
    include/databasemanager.h
    ui/mainwindow.ui
    ui/vaultwindow.ui
    ui/registerwindow.ui
    assets.qrc
)

target_link_libraries(SafePass PRIVATE Qt6::Widgets Qt6::Sql)