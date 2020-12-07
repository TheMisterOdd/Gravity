#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

#include "Input.h"


class Window
{
public:

	/* main functions */
	Window(int width, int height, std::string title, bool fullscreen);

	~Window();

	bool is_running();

	/* setters and getters */
	int get_width();

	int get_height();

	double get_delta();

	bool get_fullscreen();

	GLFWwindow* get_handeler();

	const GLFWvidmode* get_vidmode();

	Input* get_input();

	void set_size(int width, int height);

	void set_should_close(bool value);

private: 

	int m_Width;

	int m_Height;

	GLFWwindow* m_Window;

	const GLFWvidmode* m_VideoMode;

	std::unique_ptr<Input> m_Input;

	double m_Delta;

	double m_LastTime;

	bool m_Fullscreen;
};

