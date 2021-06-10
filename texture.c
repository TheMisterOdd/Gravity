
#include "texture.h"

#include <stdio.h>
#include <malloc.h>
#include <time.h>

void mk_texture2d(texture_t This, const char* path) {
	glGenTextures(1, &This[3]);

	stbi_set_flip_vertically_on_load(1);
	unsigned char* img = stbi_load(path, (int*)&This[0], (int*)&This[1], (int*)&This[2], STBI_rgb_alpha);
	stbi_set_flip_vertically_on_load(0);

	if (img == NULL)
	{
		fprintf(stderr, "[Error]: Cannot load given file '%s'\n", path);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, This[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)This[0], (int)This[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(img);
}

void mk_texture2d_from_bytes(texture_t This, unsigned char* raw_data, int width, int height) {
	This[0] = width;
	This[1] = height;
	glGenTextures(1, &This[3]);

	if (!raw_data)
	{
		fprintf(stderr, "[Error]: Cannot load given texture\n");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, This[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)This[0], (int)This[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, raw_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}

INSIGHT_API void mk_texture_skybox(texture_t This, char** faces_files, unsigned int lenght) {
	printf("Loading new skybox texture...\n");
	unsigned char* img = NULL;

	glGenTextures(1, &This[3]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, This[3]);

	unsigned int i;
	for (i = 0; i < lenght; i++)
	{
		img = stbi_load(faces_files[i], (int*)&This[0], (int*)&This[1], (int*)&This[2], STBI_rgb);
		if (!img)
		{
			fprintf(stderr, "[Error]: Cannot load given file '%s'\n", faces_files[i]);
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, (int)This[0], (int)This[1], 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		stbi_image_free(img);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void texture_bind(texture_t self, unsigned int sampler)
{	
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, self[3]);
}

void texture_unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_copy(texture_t dest, texture_t src) {
	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = src[2];
	dest[3] = src[3];
}

void texture_finalize(texture_t self) {
	glDeleteTextures(1, &self[3]);
}

int screenshot() {
	time_t t = time(NULL);
	struct tm tm;
	localtime_s(&tm, &t);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int x = viewport[0];
	int y = viewport[1];
	int width = viewport[2];
	int height = viewport[3];

	unsigned char* data = (unsigned char*)malloc((size_t)(width * height * 3)); 

	if (!data)
		return 0;

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	char* _Buffer = (char*)malloc(sizeof(char) * 512);
	sprintf(_Buffer, "%d-%d-%d_%d.%d.%02d.png", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
#ifdef _DEBUG
	printf("\nScreenshot taken: %s\n", _Buffer);
#endif
	stbi_flip_vertically_on_write(1);
	int saved = stbi_write_png(_Buffer, width, height, 3, data, 0);
	stbi_flip_vertically_on_write(0);

	free(data);
	free(_Buffer);

	return saved;
}

GLFWcursor* window_set_cursor(struct window* This, const char* path)
{
	GLFWimage* image = calloc(1, sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	GLFWcursor* cursor = glfwCreateCursor(image, 0, 0);

	glfwSetCursor(This->windowHandeler, cursor);
	free(image);

	return cursor;
}

void window_set_icon(struct window* This, const char* path)
{
	GLFWimage* image = calloc(1, sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	glfwSetWindowIcon(This->windowHandeler, 1, image);

	free(image);
}
