cmake_minimum_required(VERSION 3.22)
project(gl_exp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/)

find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(Game/include)
include_directories(Engine/include)
include_directories(${GLFW_INCLUDE_DIRS})

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    "Game/src/*.cpp"
)
file(GLOB_RECURSE E_SRC CONFIGURE_DEPENDS
    "Engine/src/*.cpp"
)
add_library(Engine SHARED ${E_SRC})
add_executable(gl_exp ${SRC})



target_link_libraries(gl_exp Engine glfw OpenGL ${GLFW_LIBRARIES} GLEW)