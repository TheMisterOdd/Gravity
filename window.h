#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "insight.h"
#include "input.h"
#include <GLFW/glfw3.h>

struct window {
	int width;

	int height;

	int fullscreen;

	double deltaTime;

	double lastTime;

	GLFWwindow* windowHandeler;

	const GLFWvidmode* videoMode;

	input_t* input;
};

/*
*/
INSIGHT_API struct window* mk_window(int width, int height, const char* title, int fullscreen);

/*
*/
INSIGHT_API int window_is_running(struct window* This);

/*! Sets new size of the window */
INSIGHT_API void window_set_size(struct window* This, int width, int height);

/*! Sets if the window should close or not */
INSIGHT_API void window_set_should_close(struct window* This, int value);

/*
*/
INSIGHT_API void window_finalize(struct window* This);

/*
*/
void (*_window_resize_callback)(GLFWwindow* window, int width, int height);

#endif /* !_WINDOW_H_ */