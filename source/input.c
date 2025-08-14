#include "../include/fern.h"

u8 IsKeyDown(Renderer *renderer, u32 keycode) {
	return glfwGetKey(renderer->glfw_window, keycode) != GLFW_RELEASE;
}
