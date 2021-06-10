#ifndef _MODEL_H_
#define _MODEL_H_

#include "insight.h"
#include <stddef.h>

/*
	Custom model declaration.
	Is an array of 4 elements:
	-	1st is the vbo id
	-	2nd is the vao id
	-	3nd is the ibo id
	-	4th is the indices count
*/
typedef unsigned int model_t[4];

/*! Returns a pointer to a model in memory with indices. */
INSIGHT_API void mk_model(model_t This, const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize);

/*! Returns a pointer to a model in memory with indices. */
INSIGHT_API void mk_model_no_indices(model_t This, const float* vertices, size_t vertexSize, const unsigned int* indices, size_t indexSize);

/*! Starts using the given model, so the user can start managing the model. */
INSIGHT_API void model_begin(model_t self);

/*! Draw the model onto screen.
NOTE: 'model_begin' and 'model_end', should be called before and after this function, respectively. */
INSIGHT_API void model_draw(model_t self, unsigned int mode);

INSIGHT_API void model_draw_no_indices(model_t self, unsigned int mode);

/*! Stops using the current binded model, so the user cannot handle it anymore. */
INSIGHT_API void model_end();

/*! Deletes the memory of the given model. */
INSIGHT_API void model_finalize(model_t self);



#endif /* _MODEL_H_ */
