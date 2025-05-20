cmake_minimum_required(VERSION 3.10)
project(MetroRouteApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск SFML и SQLite
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(SQLite3 REQUIRED)

# Подключение внутренних подпроектов
add_subdirectory(src/model)
add_subdirectory(src/database)
add_subdirectory(src/algorithm)

# Основной исполняемый файл (например, с UI)
add_executable(metro_app
    mainUi.cpp
)

# Линковка библиотек
target_link_libraries(metro_app
    PRIVATE
    MetroModel
    MetroDatabase
    PathFinder
    SQLite::SQLite3
    sfml-graphics
    sfml-window
    sfml-system
)

# Указание директории заголовков
target_include_directories(metro_app
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
