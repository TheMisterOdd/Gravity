#include "Camera.h"


Camera::Camera(int width, int height)
{
	float widthf = (float)width;
	float heightf = (float)height;
	this->m_Projection = glm::ortho(-widthf / 1000.0f, widthf / 1000.0f, -heightf / 1000.0f, heightf / 1000.0f);
	this->m_Projection = glm::scale(this->m_Projection, glm::vec3(6750.0f));
	this->m_View = glm::mat4(1.0);
}

Camera::~Camera()
{
}

glm::vec4 Camera::get_pos()
{
	return this->m_View[3];
}

void Camera::set_pos(glm::vec4 pos)
{
	this->m_View[3].x = pos.x;
	this->m_View[3].y = pos.y;
	this->m_View[3].z = pos.z;
}

void Camera::scale(glm::vec4 scale)
{
	this->m_View = glm::scale(m_View, glm::vec3(scale.x, scale.y, scale.z));
}


void Camera::update(int width, int height)
{
	float widthf = (float)width;
	float heightf = (float)height;
	this->m_Projection = glm::ortho(-widthf / 1000.0f, widthf / 1000.0f, -heightf / 1000.0f, heightf / 1000.0f);
}


glm::mat4 Camera::get_mvp(glm::mat4 model)
{
	return this->m_Projection * this->m_View * model;
}
