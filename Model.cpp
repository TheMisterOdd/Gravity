#include "Model.h"
#include <glad/glad.h>

Model::Model(const float* vertices, size_t vertSize, const unsigned int* indices, size_t indSize)
{/* Create a Vertex Array Object: */
	glGenVertexArrays(1, &this->m_Vao);
	glBindVertexArray(this->m_Vao);

	/* Generate a Vertex Buffer Object: */
	glGenBuffers(1, &this->m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

	/* Create the attributes for the shader: */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	/* Create a Index Buffer Object: */
	glGenBuffers(1, &this->m_Ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->m_Count = (unsigned int)(indSize / sizeof(unsigned int));
}

Model::Model(const float* vertices, size_t vertSize)
{
	glGenVertexArrays(1, &this->m_Vao);
	glBindVertexArray(this->m_Vao);

	/* Generate a Vertex Buffer Object: */
	glGenBuffers(1, &this->m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->m_Count = vertSize / sizeof(float) / 2;
}

Model::~Model()
{
	glDeleteVertexArrays(1, &this->m_Vao);
	glDeleteBuffers(1, &this->m_Vbo);
	glDeleteBuffers(1, &this->m_Ibo);
}

unsigned int Model::get_vao()
{
	return m_Vao;
}

void Model::bind()
{
	glBindVertexArray(this->m_Vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void Model::draw(unsigned int mode)
{
	glDrawElements(mode, this->m_Count, GL_UNSIGNED_INT, nullptr);
}

void Model::draw_without_indices(unsigned int mode)
{
	glDrawArrays(mode, 0, this->m_Count);
}

void Model::unbind()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

unsigned int Model::get_count()
{
	return this->m_Count;
}
