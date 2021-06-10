#include "model.h"

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

#include <glad/glad.h>

void mk_model(model_t This, const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize) {
	/* Create a Vertex Array Object: */
	glGenVertexArrays(1, &This[0]);
	glBindVertexArray(This[0]);

	/* Generate a Vertex Buffer Object: */
	glGenBuffers(1, &This[1]);
	glBindBuffer(GL_ARRAY_BUFFER, This[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

	/* Create the attributes for the shader: */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	/* Create a Index Buffer Object: */
	glGenBuffers(1, &This[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, This[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	This[3] = (unsigned int)(indexSize / sizeof(unsigned int));
}

INSIGHT_API void mk_model_no_indices(model_t This, const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize)
{
	glGenVertexArrays(1, &This[0]);
	glBindVertexArray(This[0]);

	/* Generate a Vertex Buffer Object: */
	glGenBuffers(1, &This[1]);
	glBindBuffer(GL_ARRAY_BUFFER, This[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	This[2] = vertexSize / sizeof(float) / 2;
}

void model_begin(model_t This) {
	glBindVertexArray(This[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void model_end() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

void model_draw(model_t This, unsigned int mode) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(mode, This[3], GL_UNSIGNED_INT, NULL);
}

void model_draw_no_indices(model_t This, unsigned int mode)
{
	glDrawArrays(mode, 0, This[2]);
}

void model_finalize(model_t This) {
	glDeleteVertexArrays(1, &This[0]);
	glDeleteBuffers(1, &This[1]);
	glDeleteBuffers(1, &This[2]);
}
