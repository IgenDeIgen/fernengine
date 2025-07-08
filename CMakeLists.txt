cmake_minimum_required(VERSION 3.10)

set(SOURCE_FILES
	"source/fern_error.c"
	"source/display.c"
	"source/model.c"
	"source/shader.c"
	"source/material.c"
	"source/loader.c"
)

add_library(fern ${SOURCE_FILES} "dependencies/glad/src/gl.c")

add_subdirectory("dependencies/glfw")

target_include_directories(fern PUBLIC "include/fern2.h" "dependencies/glfw/include" "dependencies/glad/include" "dependencies/cglm/include" "dependencies/other")

target_link_libraries(fern glfw)
