add_library(c4_core STATIC
    opening_book.cpp
    move_sorter.cpp
    transposition_table.cpp
    position.cpp
    solver.cpp
)

target_include_directories(c4_core PRIVATE ${CMAKE_SOURCE_DIR}/external/include)