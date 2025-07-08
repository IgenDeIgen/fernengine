#include "../include/fern.h"
#include "GLFW/glfw3.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

u32 renderer_should_close(renderer_t *renderer) {
	return glfwWindowShouldClose(renderer->glfw_window);
}

fern_error_t create_renderer(renderer_t* renderer, window_info_t window_info) {
	// Initializing GLFW
	if(!glfwInit()) { return error_msg(FERN_GLFW_INITIALIZATION_FAILED, NULL); }
	
	// Create GLFWwindow
	renderer->glfw_window = glfwCreateWindow(
		(s32)window_info.width,
		(s32)window_info.height,
		window_info.title,
		NULL, NULL);

	if(!renderer->glfw_window) {	
		glfwTerminate();
		return error_msg(FERN_WINDOW_CREATION_FAILED, NULL);
	}

	glfwMakeContextCurrent(renderer->glfw_window);

	// Initialize glad
	if(!gladLoadGL(glfwGetProcAddress)) {
		glfwDestroyWindow(renderer->glfw_window);
		glfwTerminate();
		return error_msg(FERN_GLAD_INITIALIZATION_FAILED, NULL);
	}

	glfwSetFramebufferSizeCallback(renderer->glfw_window, framebuffer_size_callback);
	
	glClearColor(
		window_info.clear_color.r,
		window_info.clear_color.g,
		window_info.clear_color.b,
		window_info.clear_color.a
	);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);  

	/*
	glm_mat4_identity(renderer->projection);
	glm_ortho(-((f32)window_width/2.f),(f32) window_width/2.f, -((f32)window_height/2.f),(f32) window_height/2.f, .1f, 100.f, renderer->projection);

	glm_mat4_identity(renderer->view);
	glm_translate(renderer->view, (vec3){0.f, 0.f, -3.f});
	*/
	return FERN_ERROR_OK;
}

void destroy_renderer(renderer_t *renderer) {
	glfwDestroyWindow(renderer->glfw_window);
	glfwTerminate();
}

void clear_renderer(renderer_t *renderer) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void swap_renderer(renderer_t *renderer) {
	glfwSwapBuffers(renderer->glfw_window);
}

void poll_events() {
	glfwPollEvents();
}
