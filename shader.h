#ifndef _SHADER_H_
#define _SHADER_H_

#include "insight.h"

#define DEFAULT_VS\
	"#version 330 core\n"\
	"layout(location = 0) in vec3 m_Position;"\
	"layout(location = 1) in vec2 m_TexCoords;"\
	"out vec2 TexCoords;\n"\
	"uniform mat4 MVP;\n"\
	"void main() {\n"\
	"TexCoords = m_TexCoords;\n"\
	"gl_Position = MVP * vec4(m_Position, 1.0);\n"\
	"}\n"\

#define DEFAULT_FS\
	"#version 330 core\n"\
	"in vec2 TexCoords;\n"\
	"uniform sampler2D Texture;\n"\
	"void main() {\n"\
	"vec4 texel = texture(Texture, TexCoords);\n"\
	"gl_FragColor = texel;\n"\
	"}\n"\


/*! Returns a pointer to a shader object in memory. */
INSIGHT_API unsigned int mk_shader(const char* vsSrc, const char* fsSrc);

/*! Returns a pointer to a shader object in memory. */
INSIGHT_API unsigned int mk_shader_from_path(const char* vsPath, const char* fsPath);

/*! Binds the given shader, so the user can use it. */
INSIGHT_API void shader_bind(unsigned int This);

/*! Unbinds the current binded shader, so it cannot be used anymore. */
INSIGHT_API void shader_unbind();

/*! Deletes the memory of the given shader. */
INSIGHT_API void shader_finalize(unsigned int This);


#endif
