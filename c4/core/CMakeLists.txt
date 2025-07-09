add_library(core STATIC
    opening_book.cpp
    move_sorter.cpp
    transposition_table.cpp
    position.cpp
    solver.cpp
)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/src)