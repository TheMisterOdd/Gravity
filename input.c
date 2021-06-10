#include "input.h"

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>

input_t* mk_input(GLFWwindow* window) {
	input_t* self = calloc(1, sizeof(input_t));
	assert(self);

	self->window = window;

	return self;
}

bool input_is_key_down(input_t* self, int key) {
	return glfwGetKey(self->window, key) == 1;
}

bool input_is_key_pressed(input_t* self, int key) {
	return (input_is_key_down(self, key) && !self->keys[key]);
}

bool input_is_key_released(input_t* self, int key) {
	return (!input_is_key_down(self, key) && self->keys[key]);
}

bool input_is_mouse_button_down(input_t* self, int button) {
	return glfwGetMouseButton(self->window, button) == 1;
}

bool input_is_mouse_button_pressed(input_t* self, int button) {
	return (input_is_mouse_button_down(self, button) && !self->mouseButtons[button]);
}

bool input_is_mouse_button_released(input_t* self, int button) {
	return (!input_is_mouse_button_down(self, button) && self->mouseButtons[button]);
}

void input_get_mouse_pos(input_t* self, double* x, double* y) {
	glfwGetCursorPos(self->window, x, y);
}

void input_update(input_t* self) {
	memset(self->keys, 0, sizeof(self->keys));
	memset(self->mouseButtons, 0, sizeof(self->mouseButtons));
}

void input_finalize(input_t* self) {
	self->window = NULL;
	free(self);
	self = NULL;
}