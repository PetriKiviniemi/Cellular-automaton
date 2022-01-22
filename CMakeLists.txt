cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 17)
project(GameOfLife VERSION 1.0)
include_directories(include)
file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/*.hpp" "include/*.h" "include/*.hpp")


add_subdirectory("C:/glfw-src" "C:/glfw-build")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
include_directories("C:/glfw-src/include")
find_package(OpenGL REQUIRED)
add_executable(GameOfLife ${SOURCES})
target_link_libraries(GameOfLife OpenGL::GL glfw) 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
