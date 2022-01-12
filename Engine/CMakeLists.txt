cmake_minimum_required(VERSION 3.22)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../CMakeModules/)

find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(include)
include_directories(${GLFW_INCLUDE_DIRS})

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    "src/*.cpp"
)


add_executable(Engine ${SRC})

target_link_libraries(Engine glfw OpenGL ${GLFW_LIBRARIES} GLEW)