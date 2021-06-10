#include <glad/glad.h>
#include "window.h"

#include <malloc.h>
#include <stdio.h>

static void _errorCallback(int error, const char* description) {
	fprintf(stderr, "[Error]: code %d:\n%s\n", error, description);
}

struct window* mk_window(int width, int height, const char* title, int fullscreen) {
	/* allocate memory */
	struct window* This = calloc(1, sizeof(struct window));
	if (This == NULL) {
		fprintf(stderr, "[Error]: Cannot allocate memory for the new memory.\n");
		return NULL;
	}

	/* Save values */
	This->width		 = width;
	This->height	 = height;
	This->fullscreen = fullscreen;
	
	/* OpenGL context related */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create window */
	This->windowHandeler = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (This == NULL) {
		fprintf(stderr, "[Error]: Cannot create a GLFW window.\n");
		return NULL;
	}
	
	/* Set Window on the center of the screen when ever it starts */
	This->videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(This->windowHandeler, (This->videoMode->width - width) / 2, (This->videoMode->height - height) / 2);

	glfwSetFramebufferSizeCallback(This->windowHandeler, _window_resize_callback);
	glfwSetErrorCallback(_errorCallback);

	This->input = mk_input(This->windowHandeler);
	
	glfwMakeContextCurrent(This->windowHandeler);
	
	if (!gladLoadGL()) {
		fprintf(stderr, "[Error]: Could not create an OpenGL context\n");
		return NULL;
	}

	/* OpenGL Related: */
	printf(
		"OpenGL %s\nGLFW %s\n%s\n%s\n",
		glGetString(GL_VERSION),
		glfwGetVersionString(),
		glGetString(GL_RENDERER),
		glGetString(GL_VENDOR)
	);
	return This;
}

int window_is_running(struct window* This) {
	double currentTime = glfwGetTime();
	This->deltaTime = currentTime - This->lastTime;
	This->lastTime = currentTime;

	input_update(This->input);

	glFlush();
	glfwSwapBuffers(This->windowHandeler);
	glfwPollEvents();

	glfwGetFramebufferSize(This->windowHandeler, &This->width, &This->height);
	return !glfwWindowShouldClose(This->windowHandeler);
}

void window_set_size(struct window* This, int width, int height) {
	glfwSetWindowSize(This->windowHandeler, width, height);
	glfwSetWindowPos(This->windowHandeler, (This->videoMode->width - width) / 2, (This->videoMode->height - height) / 2);
}

void window_set_should_close(struct window* This, int value) {
	glfwSetWindowShouldClose(This->windowHandeler, value);
}

void window_finalize(struct window* This) {
	This->videoMode = NULL;
	glfwDestroyWindow(This->windowHandeler);
	input_finalize(This->input);
	free(This);
}
