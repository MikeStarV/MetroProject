add_library(MetroModel STATIC
    station.cpp
)

target_include_directories(MetroModel
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/model
)
