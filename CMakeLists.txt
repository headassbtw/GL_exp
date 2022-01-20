set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 3.4)
project(gl_exp)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)


include_directories(Game/include)
include_directories(Engine/include)
include_directories(external/include)

include_directories(${GLFW_INCLUDE_DIRS})

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    "Game/src/*.cpp"
)
file(GLOB_RECURSE E_SRC CONFIGURE_DEPENDS
    "Engine/src/*.cpp"
)

add_library(Engine SHARED ${E_SRC})
add_executable(gl_exp ${SRC})

add_link_options(Engine SHARED)
add_link_options(squirrel SHARED)

target_link_libraries(gl_exp Engine SDL2 squirrel sqstdlib pthread OpenGL ${GLFW_LIBRARIES} GLEW)