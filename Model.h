#pragma once
class Model
{
public:
	Model(const float* vertices, size_t vertSize, const unsigned int* indices, size_t indSize);

	Model(const float* vertices, size_t vertSize);

	~Model();

	unsigned int get_vao();

	void bind();

	void draw(unsigned int mode);

	void draw_without_indices(unsigned int mode);

	void unbind();

	unsigned int get_count();

private:
	unsigned int m_Vao;

	unsigned int m_Vbo;

	unsigned int m_Ibo;

	unsigned int m_Count;
};

