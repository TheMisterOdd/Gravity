#pragma once

#include <glad/glad.h>
#include <string>

#define DEFAULT_VS\
	"#version 330 core\n"\
	"layout(location = 0) in vec3 m_Position;"\
	"layout(location = 1) in vec2 m_TexCoords;"\
	"out vec2 TexCoords;\n"\
	"uniform mat4 mvp;\n"\
	"void main() {\n"\
	"TexCoords = m_TexCoords;\n"\
	"gl_Position = mvp * vec4(m_Position, 1.0);\n"\
	"}\n"\

#define DEFAULT_FS\
	"#version 330 core\n"\
	"in vec2 TexCoords;\n"\
	"uniform sampler2D Texture;\n"\
	"void main() {\n"\
	"vec4 texel = texture(Texture, TexCoords);\n"\
	"gl_FragColor = texel;\n"\
	"}\n"\




class Shader
{
public:
	Shader(std::string vsSrc, std::string fsSrc);

	~Shader();

	void bind();

	void unbind();

	unsigned int get_program();

	int m_MvpLoc = -1;

private:
	unsigned int m_Program;
};

