cmake_minimum_required(VERSION 3.22.1)
project(VideoGen C CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/glfw)
include_directories(lib/glfw)
add_subdirectory(lib/FFmpeg)
include_directories(lib/FFmpeg)

list(APPEND SOURCES
    src/main.cpp
    src/load_frame.cpp
)
add_executable(VideoGen ${SOURCES})
target_link_libraries(VideoGen FFmpeg glfw ${EXTRA_LIBS})
find_package(OpenGL REQUIRED)
target_link_libraries(VideoGen OpenGL::GL)