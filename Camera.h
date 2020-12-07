#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 

class Camera
{
public:
	Camera(int width, int height);

	~Camera();

	glm::vec4 get_pos();

	void set_pos(glm::vec4 pos);

	void scale(glm::vec4 scale);

	void update(int width, int height);

	glm::mat4 get_mvp(glm::mat4 model);

	glm::mat4 m_Projection;

	glm::mat4 m_View;

};

