#include "Shader.h"

static unsigned int _shader(unsigned int type, const char* src)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	int status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);

		char* err = (char*)malloc(sizeof(char) * status);
		glGetShaderInfoLog(shader, status, &status, err);

		printf((type == GL_VERTEX_SHADER) ? "\nVERTEX_SHADER_ERROR: \n%s\n" : "\nFRAGMENT_SHADER_ERROR: \n%s\n", err);
	}

	return shader;
}

char* _read_file(const char* path)
{

	long length;
	FILE* f = NULL;
	fopen_s(&f, path, "rb"); /* was "rb" */
	if (f == NULL)
	{
		fprintf(stderr, "[Error]: Cannot allocate memory for new shader.\n");
		exit(-1);
	}

	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	if (buffer)
	{
		fread(buffer, sizeof(char), length, f);
	}

	fclose(f);

	buffer[length] = '\0';

	return buffer;
}


Shader::Shader(std::string vsSrc, std::string fsSrc) 
{
	this->m_Program = glCreateProgram();

	unsigned int vs = _shader(GL_VERTEX_SHADER, vsSrc.c_str());
	unsigned int fs = _shader(GL_FRAGMENT_SHADER, fsSrc.c_str());

	glAttachShader(this->m_Program, vs);
	glAttachShader(this->m_Program, fs);
	glLinkProgram(this->m_Program);
	glValidateProgram(this->m_Program);

	glDetachShader(this->m_Program, vs);
	glDetachShader(this->m_Program, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader()
{
	glDeleteProgram(this->m_Program);
}

void Shader::bind()
{
	glUseProgram(this->m_Program);
}

void Shader::unbind()
{
	glUseProgram(0);
}

unsigned int Shader::get_program()
{
	return this->m_Program;
}
