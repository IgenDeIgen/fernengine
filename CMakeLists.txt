cmake_minimum_required(VERSION 3.10)

add_library(fern "source/engine.c" "source/model.c" "source/shader.c" "source/transform.c"  "source/loader.c" "source/primitives.c" "source/texture.c" "source/input.c" "dependencies/glad/src/gl.c")

add_subdirectory("dependencies/glfw")

target_include_directories(fern PUBLIC "dependencies/glfw/include" "dependencies/glad/include" "dependencies/cglm/include" "dependencies/other")

target_link_libraries(fern glfw)
