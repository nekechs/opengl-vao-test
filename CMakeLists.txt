cmake_minimum_required(VERSION 3.22)
add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)
add_link_options(-Wall)

project(vert_array)

add_subdirectory(utils)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME} main.c shaderparse.c)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} GLEW)
target_link_libraries(${PROJECT_NAME} utils)