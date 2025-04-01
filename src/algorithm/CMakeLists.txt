add_library(PathFinder STATIC
    pathfinder.cpp
)

target_link_libraries(PathFinder
    PRIVATE
    MetroModel
)

target_include_directories(PathFinder
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/algorithm
    ${CMAKE_SOURCE_DIR}/include/model
)
