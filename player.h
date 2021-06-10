#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <cglm/cglm.h>
#include <string.h>

#include "texture.h"
#include "celestial_body.h"

typedef struct {

	vec3 position;

	vec3 velocity;

	vec3 acceleration;

	float mass;

	texture_t tex;

	mat4 object;

	float trails[1 << 16];
	
	size_t trails_len;

} player_t;

static void mk_player(player_t* This, vec3 initial_pos, vec3 initial_vel, texture_t tex, float mass) {
	memset(This, 0, sizeof(player_t));

	glm_mat4_identity(This->object);

	glm_vec3_copy(initial_pos, This->position);

	glm_vec3_copy(initial_vel, This->velocity);

	memcpy(This->tex, tex, sizeof(texture_t));

	This->mass = mass;
}

static void player_update_velocity(player_t* This, body_t bodies[], int len, float time_step)
{
	int i;
	for (i = 0; i < len; i++)
	{
		vec3 distance, forceDir, velDir;

		glm_vec3_sub(bodies[i].position, This->position, distance);
		glm_normalize_to(distance, forceDir);
		glm_normalize_to(This->velocity, velDir);

		float sqrRadius = glm_dot(distance, distance);
		glm_vec3_scale(forceDir, G * bodies[i].mass / sqrRadius, This->acceleration);

		if (sqrtf(sqrRadius) <= bodies[i].radius) {
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

static void player_update_position(player_t* This, float scale, float time_step) {
	glm_vec3_muladds(This->velocity, time_step, This->position); // s = s0 + v0t
	glm_translate_make(This->object, This->position);
	glm_scale(This->object, (vec3) { scale, scale, scale });
}

static void player_input(player_t* This, input_t* input, float time_step) {
	if (input_is_key_down(input, GLFW_KEY_A)) {
		This->acceleration[0] -= 200.0f;
	}

	if (input_is_key_down(input, GLFW_KEY_D)) {
		This->acceleration[0] += 200.0f;
	}

	if (input_is_key_down(input, GLFW_KEY_W)) {
		This->acceleration[1] += 200.0f;
	}

	if (input_is_key_down(input, GLFW_KEY_S)) {
		This->acceleration[1] -= 200.0f;
	}
	glm_vec3_muladds(This->acceleration, time_step, This->velocity);
}

static void player_draw_trails(player_t* This, model_t m, unsigned int shader, camera_t* cam, float time_step) {

}

static void player_draw(player_t* This, model_t m, unsigned int shader, camera_t* cam, float time_step) {

	mat4 mvp = GLM_MAT4_IDENTITY_INIT;

	shader_bind(shader);
	texture_bind(This->tex, 0);
	model_begin(m);

	camera_get_mvp(cam, This->object, mvp);
	glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, (float*)mvp);
	model_draw(m, GL_TRIANGLES);

	model_end();
	texture_unbind();
	shader_unbind();

}

#endif
