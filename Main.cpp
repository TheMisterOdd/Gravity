
#include "Window.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "CelestialBody.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>
#include <algorithm>


constexpr auto vs = R"(
#version 330 core
layout(location = 0) in vec2 m_Position;
uniform mat4 mvp;
void main() {
	gl_Position = mvp * vec4(m_Position, 0.0, 1.0);
}
)";

constexpr auto fs = R"(
#version 330 core
uniform vec3 color;
void main() {
	gl_FragColor = vec4(color, 1.0);
}
)";

int mouse_offset = 0;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
	mouse_offset = (int)yoffset;
}

int main() {
	glfwInit();
{
	std::unique_ptr<Window> window = std::unique_ptr<Window>(new Window(1280, 720, "", false));
	glfwSetScrollCallback(window.get()->get_handeler(), scroll_callback);
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

	Model m(vertices, sizeof(float) * 20, indices, sizeof(unsigned int) * 6);
	Shader s(DEFAULT_VS, DEFAULT_FS);
	s.m_MvpLoc = glGetUniformLocation(s.get_program(), "mvp");
	Shader trails_shader(vs, fs);
	trails_shader.m_MvpLoc = glGetUniformLocation(trails_shader.get_program(), "mvp");

	Camera cam(window->get_width(), window->get_height());
	Texture earthTex("data/earth.png");
	Texture moonTex("data/moon.png");
	Texture phobosTex("data/generic.png");

	// 1e14f 1e10f
	CelestialBody Earth("Earth", earthTex, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 5.972e14f, 1, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));	
	CelestialBody Moon("Moon", moonTex, glm::vec3(1000.0f, 0.0f, 0.0f), glm::vec3(0.0f, 4.0f, 0.0f), 7.348e9f, 1, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	CelestialBody Phobos("Phobos", phobosTex, glm::vec3(-120.0f, 0.0f, 0.0f), glm::vec3(0.0f, 15.0f, 0.0f), 1e3f, 1, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	std::vector<CelestialBody> bodies;
	bodies.push_back(Earth);
	bodies.push_back(Moon);
	bodies.push_back(Phobos);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window->get_handeler(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	glLineWidth(1.2f);

	double lastTime = glfwGetTime();
	while (window->is_running()) {

		float currentTime = (float)glfwGetTime();
		double dt = window->get_delta();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

		{
			cam.update(window->get_width(), window->get_height());

			glm::vec4 pos = cam.get_pos();
			if (window->get_input()->is_key_down(GLFW_KEY_ESCAPE)) {
				window->set_should_close(true);
			}

			if (window->get_input()->is_key_down(GLFW_KEY_W)) {
				pos.y -= 5.0f *dt;
			}

			if (window->get_input()->is_key_down(GLFW_KEY_S)) {
				pos.y += 5.0f * dt;
			}

			if (window->get_input()->is_key_down(GLFW_KEY_D)) {
				pos.x -= 5.0f * dt;
			}

			if (window->get_input()->is_key_down(GLFW_KEY_A)) {
				pos.x += 5.0f * dt;
			}

			glm::vec4 scale(1.0f);
			if (mouse_offset < 0.0f) {
				scale -= 100.0f * dt;
			}
			else if (mouse_offset > 0.0f) {
				scale += 100.0f * dt;
			}
			mouse_offset = 0;
			cam.scale(scale);
			cam.set_pos(pos);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Celestial Bodies");

		for (auto& body : bodies) {
			body.update_velocity(bodies, dt);

			float Velocity = body.get_velocity();
			float Acceleration = body.get_acceleration();
			ImGui::TextColored(ImVec4(body.get_orbit_color().x, body.get_orbit_color().y, body.get_orbit_color().z, body.get_orbit_color().w), "%s:", body.get_name().c_str());

			ImGui::Text("Mass: %g (kg)", body.get_mass());

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("v.x = %.2f (m/s)", body.velocity.x);
			ImGui::Text("v.y = %.2f (m/s)", body.velocity.y);
			ImGui::Text("|v| = %.2f (m/s)", Velocity);
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::Text("a.x = %.2f (m/s^2)", body.acceleration.x);
			ImGui::Text("a.y = %.2f (m/s^2)", body.acceleration.y);
			ImGui::Text("|a| = %.2f (m/s^2)", Acceleration);



			ImGui::Spacing();
			ImGui::Spacing();

			for (auto& _body : bodies) {
				if (body != _body) {

					float radius = body.radius_to(_body);
					ImGui::Text("Distance to %s = %.2f (m)", _body.get_name().c_str(), radius);

					float NormalAcceleration = (Velocity * Velocity) / radius;
					ImGui::Text("Normal Acceleration to %s = %.2f (m/s^2)", _body.get_name().c_str(), NormalAcceleration);
				}
				ImGui::Spacing();
				ImGui::Spacing();
			}

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

				
		}

		for (auto& body : bodies) {
			body.update_positon(dt);
			body.draw_trails(trails_shader, cam);
			body.draw(m, s, cam, 1.0f);
		}

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}	
	glfwTerminate();
}