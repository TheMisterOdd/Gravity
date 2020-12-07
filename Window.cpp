#include "Window.h"

#include <iostream>


static void _errorCallback(int error, const char* description) 
{
	std::cerr << "[Error]: code " << error << ":\n" << description << std::endl;
}

static void resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

Window::Window(int width, int height, std::string title, bool fullscreen)
{
	/* Save values */
	this->m_Width = width;
	this->m_Height = height;
	this->m_Fullscreen = fullscreen;

	/* OpenGL context related */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	/* Create window */
	this->m_Window = glfwCreateWindow(width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (this->m_Window == NULL) {
		fprintf(stderr, "[Error]: Cannot create a GLFW window.\n");
		glfwTerminate();
		return;
	}

	/* Set Window on the center of the screen when ever it starts */
	this->m_VideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(this->m_Window, (this->m_VideoMode->width - width) / 2, (this->m_VideoMode->height - height) / 2);

	glfwSetFramebufferSizeCallback(this->m_Window, resize_callback);
	glfwSetErrorCallback(_errorCallback);

	this->m_Input = std::unique_ptr<Input>(new Input(m_Window));

	glfwMakeContextCurrent(this->m_Window);

	if (!gladLoadGL()) {
		fprintf(stderr, "[Error]: Could not create an OpenGL context\n");
		return;
	}

	/* OpenGL Related: */
#ifdef _DEBUG
	printf(
		"OpenGL %s\nGLFW %s\n%s\n%s\n",
		glGetString(GL_VERSION),
		glfwGetVersionString(),
		glGetString(GL_RENDERER),
		glGetString(GL_VENDOR)
	);
#endif
}

Window::~Window()
{
	this->m_VideoMode = NULL;
	glfwDestroyWindow(this->m_Window);
	//input_finalize(This->input);
}

bool Window::is_running()
{
	double currentTime = glfwGetTime();
	this->m_Delta = currentTime - this->m_LastTime;
	this->m_LastTime = currentTime;

	this->m_Input.get()->update();

	glFlush();
	glfwSwapBuffers(this->m_Window);
	glfwPollEvents();

	glfwGetFramebufferSize(this->m_Window, &this->m_Width, &this->m_Height);

	return !glfwWindowShouldClose(this->m_Window);
}

int Window::get_width()
{
	return this->m_Width;
}

int Window::get_height()
{
	return this->m_Height;
}

double Window::get_delta()
{
	return this->m_Delta;
}

bool Window::get_fullscreen()
{
	return this->m_Fullscreen;
}

GLFWwindow* Window::get_handeler()
{
	return this->m_Window;
}

const GLFWvidmode* Window::get_vidmode()
{
	return this->m_VideoMode;
}

Input* Window::get_input()
{
	return this->m_Input.get();
}

void Window::set_size(int width, int height)
{
	glfwSetWindowSize(this->m_Window, width, height);
	glfwSetWindowPos(this->m_Window, (this->m_VideoMode->width - width) / 2, (this->m_VideoMode->height - height) / 2);
}

void Window::set_should_close(bool value)
{
	glfwSetWindowShouldClose(this->m_Window, value);
}


