#ifndef _CELESTIAL_BODY_H_
#define _CELESTIAL_BODY_H_

#define G (float)6.674384e-11f
#define AU (float)149597900000.0f

#define len(x) ((size_t)(sizeof(x) / sizeof(x[0])))

#include <malloc.h>
#include <string.h>

#include <cglm/cglm.h>

#include "window.h"
#include "texture.h"
#include "model.h"
#include "shader.h"
#include "camera.h"

typedef struct {

	texture_t texture;

	vec3 position;

	vec3 velocity;

	vec3 acceleration;

	char name[64];

	float mass;

	float radius;

	mat4 object;

	double last_time;

} body_t;

static void mk_body(body_t* This, const char* name, texture_t tex, vec3 initial_pos, vec3 initial_velocity, float mass, float radius /* add orbit color */)
{
	memset(This, 0, sizeof(body_t));
	strcpy(This->name, name);
	memcpy(This->texture, tex, sizeof(texture_t));


	glm_vec3_copy(initial_pos, This->position);
	glm_vec3_copy(initial_velocity, This->velocity);

	This->mass = mass;
	This->radius = radius;

	glm_mat4_identity(This->object);
}

static void update_velocity(body_t* This, body_t bodies[], int len, float time_step)
{
	int i;
	for (i = 0; i < len; i++) 
	{
		if ((uintptr_t)(This) != (uintptr_t)(&bodies[i])) 
		{
			vec3 distance, forceDir, velDir;

			glm_vec3_sub(bodies[i].position, This->position, distance);
			glm_normalize_to(distance, forceDir);
			glm_normalize_to(This->velocity, velDir);

			float sqrRadius = glm_dot(distance, distance);
			glm_vec3_scale(forceDir, G * bodies[i].mass / sqrRadius, This->acceleration);


			if (sqrtf(sqrRadius) <= bodies[i].radius + This->radius) {

				This->position[0] = bodies[i].position[0] - (forceDir[0] * bodies[i].radius);
				This->position[1] = bodies[i].position[1] - (forceDir[1] * bodies[i].radius);
				This->position[2] = bodies[i].position[2] - (forceDir[2] * bodies[i].radius);

				This->acceleration[0] += -velDir[0] * fabsf(This->acceleration[0]) * 10.0f;
				This->acceleration[1] += -velDir[1] * fabsf(This->acceleration[1]) * 10.0f;
				This->acceleration[2] += -velDir[2] * fabsf(This->acceleration[2]) * 10.0f;
			}
			glm_vec3_muladds(This->acceleration, time_step, This->velocity);
		}

	}
}

static void update_position(body_t* This, float time_step) {
	glm_vec3_muladds(This->velocity, time_step, This->position); // s = s0 + v0t
	glm_translate_make(This->object, This->position);
	glm_scale(This->object, (vec3) { This->radius, This->radius, 0.0f });
}

static void draw(body_t* This, struct window* wnd, model_t m, unsigned int shader, camera_t* cam, float time_step) {
	
	/* check if tile is inside the frustrum. */
	mat4 mvp = GLM_MAT4_IDENTITY_INIT;
	
	shader_bind(shader);
	texture_bind(This->texture, 0);
	model_begin(m);

	camera_get_mvp(cam, This->object, mvp);
	glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, (float*)mvp);
	model_draw(m, GL_TRIANGLES);

	model_end();
	texture_unbind();
	shader_unbind();

}

static void draw_trails(body_t* This) {

}

static void body_finalize(body_t* This) {
	memset(This, 0, sizeof(body_t));
}

#endif
