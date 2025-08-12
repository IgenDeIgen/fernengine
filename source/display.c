#include "../include/fern.h"
#include "GLFW/glfw3.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

u32 RendererShouldClose(Renderer *renderer) {
	return glfwWindowShouldClose(renderer->glfw_window);
}

frnError_t CreateRenderer(Renderer* renderer, WindowInfo window_info) {
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

void DestroyRenderer(Renderer *renderer) {
	glfwDestroyWindow(renderer->glfw_window);
	glfwTerminate();
}

void ClearRenderer(Renderer *renderer) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SwapRenderer(Renderer *renderer) {
	glfwSwapBuffers(renderer->glfw_window);
}

void PollEvents() {
	glfwPollEvents();
}
