#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>
#include "window.h"
#include "insight.h"

#include <stb_image.h>
#include <stb_image_write.h>


typedef unsigned int texture_t[4];
/*
*/
INSIGHT_API void mk_texture2d(texture_t This, const char* path);

/*
*/
INSIGHT_API void mk_texture2d_from_bytes(texture_t This, unsigned char* raw_data, int width, int height);

/*
*/
INSIGHT_API void mk_texture_skybox(texture_t This, char** faces_files, unsigned int lenght);


/*
*/
INSIGHT_API void texture_bind(texture_t self, unsigned int sampler);

/*
*/
INSIGHT_API void texture_unbind();

/*
*/
INSIGHT_API void texture_copy(texture_t dest, texture_t src);

/*
*/
INSIGHT_API void texture_finalize(texture_t self);

/*
*/
INSIGHT_API int screenshot();

/*! Sets window cursor */
INSIGHT_API GLFWcursor* window_set_cursor(struct window* self, const char* path);

/*! Sets window icon */
INSIGHT_API void window_set_icon(struct window* self, const char* path);



#endif /* _TEXTURE_H_ */