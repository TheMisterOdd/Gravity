#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/mat4.h>

#include "window.h"
#include "model.h"
#include "shader.h"
#include "texture.h"

#include "celestial_body.h"
#include "camera.h"
#include "player.h"

#include "deps/nuklear/nuklear.h"
#include "deps/nuklear/nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

bool window_has_resized = true;
void viewport(GLFWwindow* window, int fbW, int fbH) {
	window_has_resized = true;
	glViewport(0, 0, fbW, fbH);
}

int mouse_offset = 0;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouse_offset = (int)yoffset;
}

void time(double seconds, double* years, double* days, double* hours, double* minutes, double* new_seconds) {
	*years = seconds / (3600.0f * 24.0f * 365.0f);
	*days = modf(*years, years) * 365.0f;
	*hours = modf(*days, days) * 24.0f;
	*minutes = modf(*hours, hours) * 60.0f;
	*new_seconds = modf(*minutes, minutes) * 60.0f;
}

int main(void) {

	if (!glfwInit()) {
		fprintf(stderr, "[Error]: Cannot create a GLFW context!\n");
		return -1;
	}
	_window_resize_callback = viewport;
	struct window* wnd = mk_window(1280, 720, "", false);
	struct nk_glfw glfw = { 0 };
	struct nk_context* ctx;

	ctx = nk_glfw3_init(&glfw, wnd->windowHandeler, NK_GLFW3_INSTALL_CALLBACKS);
	/* Load Fonts: if none of these are loaded a default font will be used  */
	/* Load Cursor: if you uncomment cursor loading please hide the cursor */
	{struct nk_font_atlas* atlas;
	nk_glfw3_font_stash_begin(&glfw, &atlas);
	/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
	/*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
	/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
	/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
	/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
	/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
	nk_glfw3_font_stash_end(&glfw);
	/*nk_style_load_all_cursors(ctx, atlas->cursors);*/
	/*nk_style_set_font(ctx, &droid->handle);*/}

	glfwSetScrollCallback(wnd->windowHandeler, scroll_callback);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	const float vertices[] = {
		/* Positions:				Textures: */
		-1.0F, -1.0F, +0.0f,		+0.0F, +0.0F,
		+1.0F, -1.0F, +0.0f,		+1.0F, +0.0F,
		+1.0F, +1.0F, +0.0f,		+1.0F, +1.0F,
		-1.0F, +1.0F, +0.0f,		+0.0F, +1.0F
	};

	const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	model_t m;
	mk_model(m, vertices, sizeof(float) * 20, indices, sizeof(unsigned int) * 6);

	unsigned int shader = mk_shader(DEFAULT_VS, DEFAULT_FS);

	texture_t earthTex, moonTex, generic, blackhole, sun, rocket, asteroid1, asteroid2;
	mk_texture2d(earthTex, "data/earth.png");
	mk_texture2d(moonTex, "data/Moon.png");
	mk_texture2d(generic, "data/body.png");
	mk_texture2d(blackhole, "data/blackhole.png");
	mk_texture2d(sun, "data/sun.png");
	mk_texture2d(rocket, "data/rocket.png");
	mk_texture2d(asteroid1, "data/asteroid1.png");
	mk_texture2d(asteroid2, "data/asteroid2.png");

	player_t player; // 75.46247033f
	mk_player(&player, (vec3) { 700.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 75.46247033f, 0.0f }, rocket, 1.0f);

	body_t Earth, Moon, Asteroid1;
	mk_body(&Earth, "Earth", generic, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, 5.9724e16f, 637.81f);
	mk_body(&Moon, "Brisio", generic, (vec3) { 10000.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 19.96549298f, 0.0f }, 7.332521485e15, 173.81f);
	mk_body(&Asteroid1, "IO_10", asteroid1, (vec3) { -10000.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 19.96549298f, 0.0f }, 0.0f, 173.81f);
	//mk_body(&Asteroid2, "#20", asteroid2, (vec3) { -800.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 19.96549298f, 0.0f }, 0.0f, 173.81f);


	#define BODIES 3
	body_t bodies[BODIES];
	memcpy(&bodies[0], &Earth, sizeof(body_t));
	memcpy(&bodies[1], &Moon, sizeof(body_t));
	memcpy(&bodies[2], &Asteroid1, sizeof(body_t));
	//memcpy(&bodies[3], &Asteroid2, sizeof(body_t));

	camera_t cam;
	mk_camera(&cam, (vec3) { 0.0f, 0.0f, 100.0f });

	double simTime = 0.0f, timeStep = 1.0f, lastTime = 0.0, currentTime = 0.0f;

	enum timeSpeed {
		PAUSE,
		X0_5,
		X1,
		X5,
		X10,
		X100,
		X1000,
		X10000,
		X100000
	};
	int timeSpeed = X1;

	glfwSwapInterval(0);
	glfwSetTime(0.0);
	while (window_is_running(wnd)) {

		currentTime = glfwGetTime();
		float dt = (float)wnd->deltaTime;

		switch (timeSpeed)
		{
		case PAUSE:
			timeStep = 0;
			break;
		case X0_5:
			timeStep = dt * 0.25f;
			break;
		case X1:
			timeStep = dt;
			break;
		case X5:
			timeStep = 2.0f * dt;
			break;
		case X10:
			timeStep = 4.0f * dt;
			break;
		case X100:
			timeStep = 6.0f * dt;
			break;
		case X1000:
			timeStep = 8.0f * dt;
			break;
		case X10000:
			timeStep = 10.0f * dt;
			break;
		case X100000:
			timeStep = 100.0f * dt;
			break;
		}

		simTime += timeStep;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(26.0f / 255.0f, 24.0f / 255.0f, 30.0f / 255.0f, 1.0f);

		/* input */
		{
			camera_update(&cam, wnd->width, wnd->height);
			//glm_rotate(cam.view, M_PI_2, player.position);
			glm_vec3_lerp(cam.position, (vec3) { player.position[0], player.position[1], cam.position[2] + player.position[2] }, 5.25f * dt, cam.position);
			if (input_is_key_pressed(wnd->input, GLFW_KEY_ESCAPE)) {
				window_set_should_close(wnd, GLFW_TRUE);
			}
			if (mouse_offset == -1) {
				cam.position[2] += 100000.0f * (float)dt;
			}
			else if (mouse_offset == 1) {
				cam.position[2] -= 100000.0f * (float)dt;
			}
			mouse_offset = 0;
		}

		int i;
		nk_glfw3_new_frame(&glfw);
		if (nk_begin(ctx, "Celestial Bodies", nk_rect(50, 50, 420, 250),
			NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
			NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
		{
			char buffer[512];
			int len;

			nk_layout_row_static(ctx, 30, 300, 1);
			len = snprintf(buffer, 512, "Resolution %dx%d, frames %.2f", wnd->width, wnd->height, 1.0f / dt);
			nk_text(ctx, buffer, len, NK_TEXT_LEFT);

			double _year = 0.0, _days = 0.0, _hour = 0.0, _min = 0.0, _sec = 0.0;
			time(currentTime, &_year, &_days, &_hour, &_min, &_sec);
			len = snprintf(buffer, 512, "Real Time: %.0fyears %.0fdays %.0fh %.0fmin %.0fs", _year, _days, _hour, _min, _sec);
			nk_text(ctx, buffer, len, NK_TEXT_LEFT);

			time(simTime, &_year, &_days, &_hour, &_min, &_sec);
			len = snprintf(buffer, 512, "Simulation Time: %.0fyears %.0fdays %.0fh %.0fmin %.0fs", _year, _days, _hour, _min, _sec);
			nk_text(ctx, buffer, len, NK_TEXT_LEFT);

			nk_layout_row_static(ctx, 30, 40, 9);
			if (nk_button_label(ctx, "Pause")) {
				timeSpeed = PAUSE;
			}
			if (nk_button_label(ctx, "x0.5")) {
				timeSpeed = X0_5;
			}
			if (nk_button_label(ctx, "x1")) {
				timeSpeed = X1;
			}
			if (nk_button_label(ctx, "x2")) {
				timeSpeed = X5;
			}
			if (nk_button_label(ctx, "x4")) {
				timeSpeed = X10;
			}
			if (nk_button_label(ctx, "x6")) {
				timeSpeed = X100;
			}
			if (nk_button_label(ctx, "x8")) {
				timeSpeed = X1000;
			}
			if (nk_button_label(ctx, "x10")) {
				timeSpeed = X10000;
			}
			if (nk_button_label(ctx, "x100")) {
				timeSpeed = X100000;
			}
			
			nk_spacing(ctx, 0);
			for (i = 0; i < BODIES; i++) {
				nk_layout_row_dynamic(ctx, 25, 1);
				len = snprintf(buffer, 512, "%s:", bodies[i].name);
				nk_label_colored(ctx, buffer, NK_TEXT_LEFT, nk_rgb(0, 255, 255));
				len = snprintf(buffer, 512, "Mass %g (kg)", bodies[i].mass);

				nk_text(ctx, buffer, len, NK_TEXT_LEFT);
				len = snprintf(buffer, 512, "Acceleration %.2fi + %.2fj + %.2fk (m/s^2)", bodies[i].acceleration[0], bodies[i].acceleration[1], bodies[i].acceleration[2]);
				nk_text(ctx, buffer, len, NK_TEXT_LEFT);
				len = snprintf(buffer, 512, "|a| = %.2f (m/s)", glm_vec3_norm(bodies[i].acceleration));

				nk_text(ctx, buffer, len, NK_TEXT_LEFT);
				len = snprintf(buffer, 512, "Velocity %.2fi + %.2fj + %.2fk (m/s)", bodies[i].velocity[0], bodies[i].velocity[1], bodies[i].velocity[2]);
				nk_text(ctx, buffer, len, NK_TEXT_LEFT);
				len = snprintf(buffer, 512, "|v| = %.2f (m/s)", glm_vec3_norm(bodies[i].velocity));
				nk_text(ctx, buffer, len, NK_TEXT_LEFT);
				

				int j;
				for (j = 0; j < BODIES; j++) {
					if ((uintptr_t)(&bodies[i]) != (uintptr_t)(&bodies[j]))
					{
						vec3 distance;
						glm_vec3_sub(bodies[j].position, bodies[i].position, distance);
						float _distance = glm_vec3_norm(distance);

						float OrbPeriond = 2.0f * M_PI * sqrtf((_distance * _distance * _distance) / (G * (bodies[i].mass + bodies[j].mass)));
						time(OrbPeriond, &_year, &_days, &_hour, &_min, &_sec);

						len = snprintf(buffer, 512, "Distance to %s = %g (m)", bodies[j].name, _distance);
						nk_text(ctx, buffer, len, NK_TEXT_LEFT);
						len = snprintf(buffer, 512, "Orb.Period around %s: %.0fyears %.0fdays %.0fh %.0fmin %.0fs", bodies[j].name, _year, _days, _hour, _min, _sec);
						nk_text(ctx, buffer, len, NK_TEXT_LEFT);

					}
				}
				nk_spacing(ctx, 0);
			}
		}
		nk_end(ctx);

		
		for (i = 0; i < BODIES; i++) {
			update_velocity(&bodies[i], bodies, BODIES, timeStep);
			update_position(&bodies[i], timeStep);
			draw(&bodies[i], wnd, m, shader, &cam, timeStep);
		}

		player_input(&player, wnd->input, timeStep);
		player_update_velocity(&player, bodies, BODIES, timeStep);
		player_update_position(&player, 10.0f, timeStep);
		player_draw(&player, m, shader, &cam, timeStep);

		nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
	}

	nk_glfw3_shutdown(&glfw);

	window_finalize(wnd);
	model_finalize(m);
	body_finalize(&Earth);
	body_finalize(&Moon);
	glfwTerminate();


	return 0;
}