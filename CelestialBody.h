#pragma once

#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <math.h>

#define G (float)6.74e-11f

class CelestialBody {
public:

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	

	CelestialBody(std::string name, Texture& tex, glm::vec3 initialPos, glm::vec3 initialVelocity, float mass, float scale, glm::vec4 orbitColor = glm::vec4(1.0f)) 
		: name(name), tex(tex) {

		this->position = initialPos;
		this->initialPos = initialPos;
		this->velocity = initialVelocity;
		this->initialVelocity = initialVelocity;
		this->acceleration = glm::vec3(0.0f);

		this->color = orbitColor;

		this->mass = mass;
		this->scale = scale;
		this->object = glm::mat4(1.0f);
	}


	void update_velocity(std::vector<CelestialBody>& bodies, float timeStep) {
		double currentTime = glfwGetTime();

		for (auto& body : bodies) {
			if (body != *this) {

				float a = (position.x - body.position.x);
				float b = (position.y - body.position.y);
				float sqrRadius = a * a + b * b;
				float radius = glm::sqrt(sqrRadius);

				glm::vec3 forceDir = glm::normalize(body.position - this->position);
				glm::vec3 force = (forceDir * G * body.mass) / sqrRadius;	// F = m * a (mass cancelled)
				this->acceleration = force;									// a = F / m
				this->velocity += this->acceleration * timeStep;			// v = v0 + at
			}
		}
	}

	float radius_to(CelestialBody& body) {
		float sqrRadius = (position.x - body.position.x) * (position.x - body.position.x) + (position.y - body.position.y) * (position.y - body.position.y);
		return glm::sqrt(sqrRadius);
	}

	void update_positon(float timeStep) {
		//this->position += 0.5f * this->velocity * timeStep * timeStep; // s = s0 + v0t + (1/2)a * t^2
		this->position += this->velocity * timeStep; // more fun, less realistic
		this->object = glm::translate(glm::mat4(1.0f), this->position);
		this->object = glm::scale(this->object, glm::vec3(this->scale, this->scale, 0.0f));
	}

	void draw(Model& m, Shader& shader, Camera& cam, float scale) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		shader.bind();
		tex.bind(0);
		m.bind();

		this->object = glm::scale(this->object, glm::vec3(scale, scale, 0.0f));
		glUniformMatrix4fv(shader.m_MvpLoc, 1, GL_FALSE, glm::value_ptr(cam.get_mvp(object)));
		m.draw(GL_TRIANGLES);
		m.unbind();
		tex.unbind();
		shader.unbind();
		glDisable(GL_BLEND);
	}

	void draw_trails(Shader& trails_shader, Camera& cam) {
		
		if (trails.size() > 4096 * 4096) {
			trails.erase(trails.begin(), trails.end());
		}

		trails.push_back(this->position.x);
		trails.push_back(this->position.y);

		if (!trails.empty()) {
			float* lines = &trails[0];
			Model line(lines, sizeof(float) * trails.size());
			line.bind();
			trails_shader.bind();
			glUniform3f(glGetUniformLocation(trails_shader.get_program(), "color"), color.x, color.y, color.z);
			glUniformMatrix4fv(trails_shader.m_MvpLoc, 1, GL_FALSE, glm::value_ptr(cam.get_mvp(glm::mat4(1.0f))));
			line.draw_without_indices(GL_LINE_STRIP);
			trails_shader.unbind();
			line.unbind();
		}
		
	}

	float get_distance_from_camera(Camera& cam) {
		float a = (cam.get_pos().x - this->position.x);
		float b = (cam.get_pos().y - this->position.y);
		return glm::sqrt(a * a + b * b);
	}

	std::string get_name() {
		return this->name;
	}

	float get_mass() {
		return this->mass;
	}

	float get_diameter() {
		return this->scale;
	}

	float get_radius() {
		return this->scale / 2.0f;
	}

	float get_velocity() {
		return glm::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	}

	float get_acceleration() {
		return glm::sqrt(acceleration.x * acceleration.x + acceleration.y * acceleration.y);
	}

	glm::vec4 get_orbit_color() {
		return this->color;
	}

	bool operator !=(CelestialBody& b) {
		uintptr_t body_ptr = (uintptr_t)&b;
		uintptr_t this_ptr = (uintptr_t)this;
		if (body_ptr != this_ptr) {
			return true;
		}
		return false;
	}

private:
	glm::vec3 initialVelocity;
	glm::vec3 initialPos;

	std::string name;

	std::vector<float> trails;

	float mass;

	float scale;

	Texture& tex;

	glm::vec4 color;

	glm::mat4 object = glm::mat4(1.0f);
};
