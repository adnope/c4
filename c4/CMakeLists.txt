add_executable(c4)
target_sources(c4 PRIVATE main.cpp)

target_link_libraries(c4
    vendor
    core
)

if (WIN32)
    target_link_libraries(c4 ws2_32)
endif()

target_include_directories(c4 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(app)
add_subdirectory(core)