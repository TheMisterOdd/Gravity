#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input
{
public:
	Input(GLFWwindow* window);

	~Input();

	bool is_key_down(int key);

	bool is_key_pressed(int key);

	bool is_key_released(int key);

	bool is_mouse_button_down(int button);

	bool is_mouse_button_pressed(int button);

	bool is_mouse_button_released(int button);

	glm::vec2 get_mouse_pos();

	void update();


private:
	GLFWwindow* m_Window;

	bool m_Keys[GLFW_KEY_LAST];

	bool m_MouseButtons[GLFW_MOUSE_BUTTON_LAST];
};

