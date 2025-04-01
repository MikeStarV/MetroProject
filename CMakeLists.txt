cmake_minimum_required(VERSION 3.10)
project(metro_app)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

add_subdirectory(src/model)
add_subdirectory(src/database)
add_subdirectory(src/algorithm)

add_executable(metro_app main.cpp)

target_link_libraries(metro_app
    PRIVATE
    MetroModel
    MetroDatabase
    PathFinder
    SQLite::SQLite3
)

target_include_directories(metro_app
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
