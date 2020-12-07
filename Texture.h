#pragma once
#include <string>
class Texture
{
public:
	
	Texture(std::string path);

	~Texture();

	void bind(int sampler);

	void unbind();

	unsigned int get_id();

	int get_width();

	int get_height();

private:
	unsigned int m_Texture;

	int m_Width;

	int m_Height;

	int m_Channels;
};

