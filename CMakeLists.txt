cmake_minimum_required(VERSION 3.20)
project(RainbowRoad)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "*.cpp")
file(GLOB TRACK_SOURCES "track/*.cpp")

add_executable(RainbowRoad
    ${SOURCES}
    ${TRACK_SOURCES}
    includes/glad/glad.c
)

target_include_directories(RainbowRoad PRIVATE includes)

# Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(RainbowRoad PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
        GL dl pthread
    )
endif()

# Windows
if(WIN32)
    target_link_libraries(RainbowRoad PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
        opengl32
    )
endif()

add_custom_command(TARGET RainbowRoad POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:RainbowRoad>/assets
)
add_custom_command(TARGET RainbowRoad POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:RainbowRoad>/shaders
)