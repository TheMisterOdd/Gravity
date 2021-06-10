#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <cglm/cglm.h>
#include <string.h>

typedef struct {
	
	vec3 position;

	vec3 camera_front;

	vec3 camera_up;

	mat4 projection;

	mat4 view;

} camera_t;

void mk_camera(camera_t* This, vec3 initial_pos) {
	memset(This, 0, sizeof(camera_t));

	glm_mat4_identity(This->projection);
	glm_mat4_identity(This->view);
	glm_vec3_copy(initial_pos, This->position);

	glm_vec3_copy((vec3) { 0.0f, 0.0f, -1.0f }, This->camera_front);
	glm_vec3_copy((vec3) { 0.0f, 1.0f,  0.0f }, This->camera_up);

}


void camera_scale(camera_t* This, float scalar) {
	glm_mat4_scale(This->view, scalar);
}

void camera_update(camera_t* This, int width, int height) {
	glm_perspective((float)M_PI_2, (float)(width) / (float)(height), 0.1f, 1e38f, This->projection);
	glm_lookat(This->position, (vec3) { This->position[0] + This->camera_front[0], This->position[1] + This->camera_front[1], This->position[2] + This->camera_front[2] }, This->camera_up, This->view);
}

void camera_get_mvp(camera_t* This, mat4 object, mat4 dest) {
	glm_mat4_mul(This->projection, This->view, dest);
	glm_mat4_mul(dest, object, dest);
}

#endif