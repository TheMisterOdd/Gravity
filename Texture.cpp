#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(std::string path)
{
	glGenTextures(1, &this->m_Texture);

	stbi_set_flip_vertically_on_load(1);
	unsigned char* img = stbi_load(path.c_str(), &this->m_Width, &this->m_Height, &this->m_Channels, STBI_rgb_alpha);
	stbi_set_flip_vertically_on_load(0);

	if (img == NULL)
	{
		fprintf(stderr, "[Error]: Cannot load given file '%s'\n", path);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, this->m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->m_Width, this->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(img);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->m_Texture);
}

void Texture::bind(int sampler)
{
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, this->m_Texture);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::get_id()
{
	return this->m_Texture;
}

int Texture::get_width()
{
	return this->m_Width;
}

int Texture::get_height()
{
	return this->m_Height;
}
