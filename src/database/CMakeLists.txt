add_library(MetroDatabase STATIC
    metrodatabase.cpp
)

target_link_libraries(MetroDatabase
    PRIVATE
    MetroModel
    SQLite::SQLite3
)

target_include_directories(MetroDatabase
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/database
    ${CMAKE_SOURCE_DIR}/include/model
)
