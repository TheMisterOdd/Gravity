#include "Input.h"

Input::Input(GLFWwindow* window) 
{
	memset(this->m_Keys, 0, sizeof(this->m_Keys));
	memset(this->m_MouseButtons, 0, sizeof(this->m_MouseButtons));
	this->m_Window = window;
}

Input::~Input()
{
	this->m_Window = NULL;
}

bool Input::is_key_down(int key)
{
	return glfwGetKey(this->m_Window, key) == 1;
}

bool Input::is_key_pressed(int key)
{
	return (is_key_down(key) && !m_Keys[key]);
}

bool Input::is_key_released(int key)
{
	return (!is_key_down(key) && m_Keys[key]);
}

bool Input::is_mouse_button_down(int button)
{
	return glfwGetMouseButton(this->m_Window, button) == 1;
}

bool Input::is_mouse_button_pressed(int button)
{
	return (is_mouse_button_down(button) && !this->m_MouseButtons[button]);

}

bool Input::is_mouse_button_released(int button)
{
	return (!is_mouse_button_down(button) && this->m_MouseButtons[button]);

}

glm::vec2 Input::get_mouse_pos()
{
	double x, y;
	glfwGetCursorPos(this->m_Window, &x, &y);
	return glm::vec2(x, y);
}

void Input::update()
{
	memset(this->m_Keys, 0, sizeof(this->m_Keys));
	memset(this->m_MouseButtons, 0, sizeof(this->m_MouseButtons));
}

