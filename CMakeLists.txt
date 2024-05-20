cmake_minimum_required(VERSION 3.22.1)
project(VideoGen C CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/glfw)
include_directories(lib/glfw)

add_executable(VideoGen src/main.cpp)
target_link_libraries(VideoGen glfw ${EXTRA_LIBS})
find_package(OpenGL REQUIRED)
target_link_libraries(VideoGen OpenGL::GL)