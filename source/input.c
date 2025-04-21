#include "../include/fern.h"

s32 input_keydown(renderer_t* renderer, s32 keycode) {
	s32 state = glfwGetKey(renderer->glfw_window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
