cmake_minimum_required(VERSION 3.27)

project(triangle)

set(CMAKE_CXX_STANDARD 23)
set(LIBRARY_OUTPUT_PATH 
    ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_SOURCE_DIR}/bin)

add_compile_options(-Wall)
#add_compile_options(-Wall -Werror)
#add_compile_options(-fdiagnostics-color=always -fansi-escape-codes)
add_compile_definitions(PROJECT_PATH="${CMAKE_SOURCE_DIR}")

include_directories(dependencies)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

file(GLOB SRC_LIST src/*.cpp)
add_library(triangle STATIC ${SRC_LIST})

set(LIB_LIST
PRIVATE
    triangle
PUBLIC
    ${CMAKE_SOURCE_DIR}/dependencies/glad/glad.lib
    glfw
    glm
    assimp
)

add_executable(cube src/core/cube.cpp)
target_link_libraries(cube ${LIB_LIST})

add_executable(light src/core/light.cpp)
target_link_libraries(light ${LIB_LIST})

add_executable(model src/core/model.cpp)
target_link_libraries(model ${LIB_LIST})

add_executable(grass src/core/grass.cpp)
target_link_libraries(grass ${LIB_LIST})
