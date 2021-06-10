#include "shader.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

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

unsigned int mk_shader(const char* vsSrc, const char* fsSrc) {
	unsigned int shader = glCreateProgram();

	unsigned int vs = _shader(GL_VERTEX_SHADER, vsSrc);
	unsigned int fs = _shader(GL_FRAGMENT_SHADER, fsSrc);

	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDetachShader(shader, vs);
	glDetachShader(shader, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return shader;
}

unsigned int mk_shader_from_path(const char* vsPath, const char* fsPath) {
	return mk_shader(_read_file(vsPath), _read_file(fsPath));
}

void shader_bind(unsigned int This) {
	glUseProgram(This);
}

void shader_unbind() {
	glUseProgram(0);
}

INSIGHT_API void shader_finalize(unsigned int This) {
	glDeleteProgram(This);
}
