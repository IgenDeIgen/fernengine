#include "../include/fern.h"

#include <stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

s32 init_renderer(renderer_t* renderer, u32 window_width, u32 window_height, char* window_title) {
	// Initializing GLFW
	if(!glfwInit()) {
		fprintf(stderr, "Error: failed to initialize GLFW");
		return -1;
	}
	
	// Create GLFWwindow
	renderer->glfw_window = glfwCreateWindow((s32)window_width, (s32)window_height, window_title, NULL, NULL);
	if(!renderer->glfw_window) {	
		fprintf(stderr, "Error: failed to create window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(renderer->glfw_window);

	// Initialize glad
	if(!gladLoadGL(glfwGetProcAddress)) {
		fprintf(stderr, "Error: failed to load GLAD");
		glfwDestroyWindow(renderer->glfw_window);
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(renderer->glfw_window, framebuffer_size_callback);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);  


	glm_mat4_identity(renderer->projection);
	glm_ortho(-((f32)window_width/2.f),(f32) window_width/2.f, -((f32)window_height/2.f),(f32) window_height/2.f, .1f, 100.f, renderer->projection);

	glm_mat4_identity(renderer->view);
	glm_translate(renderer->view, (vec3){0.f, 0.f, -3.f});

	return 1;
}

void quit_renderer(renderer_t* renderer) {
	glfwDestroyWindow(renderer->glfw_window);
	glfwTerminate();
}


s32 renderer_should_close(renderer_t* renderer) {
	return glfwWindowShouldClose(renderer->glfw_window);
}

void poll_events() {
	glfwPollEvents();
}

void set_clear_color(renderer_t* renderer, color_rgba color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void render_clear(renderer_t* renderer) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void render_swap(renderer_t* renderer) {
	glfwSwapBuffers(renderer->glfw_window);
}

