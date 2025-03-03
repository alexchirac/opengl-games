#include "lab_m1/DroneDelivery/drone.h"
#include <iostream>

Drone::Drone() {}

Drone::~Drone() {}

float random(glm::vec2 st) {
	float val = sin(glm::dot(st,
		glm::vec2(12.9898, 78.233)))
		* 11241.5453123;

	return val - floor(val);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise(glm::vec2 st) {
	glm::vec2 i = glm::floor(st);
	glm::vec2 f = glm::fract(st);

	// Four corners in 2D of a tile
	float a = random(i);
	float b = random(i + glm::vec2(1.0, 0.0));
	float c = random(i + glm::vec2(0.0, 1.0));
	float d = random(i + glm::vec2(1.0, 1.0));

	// Smooth Interpolation
	glm::vec2 u = f * f * (3.0f - f * 2.0f);  // Cubic Hermite curve
	return glm::mix(a, b, u.x) +
		(c - a) * u.y * (1.0 - u.x) +
		(d - b) * u.x * u.y;
}

int Drone::intersectsPlatform() {
	for (int i = 0; i < platPos->size(); i++) {
		const auto& ppos = (*platPos)[i];

		if (glm::distance(ppos, pos) > 5)
			continue;

		if (ppos.y > pos.y - 0.2) {
			if (pos.x - 0.5 <= ppos.x + 0.75 &&
				pos.x + 0.5 >= ppos.x - 0.75 &&
				pos.z - 0.5 <= ppos.z + 0.75 &&
				pos.z + 0.5 >= ppos.z - 0.75) {
				speed.x = -speed.x / 4;
				speed.z = -speed.z / 4;
				speed.y = 0;
			}
		}
	}

	return 0;
}

int Drone::intersectsTree() {
	for (int i = 0; i < treesPos->size(); i++) {
		const auto& tpos = (*treesPos)[i];

		if (glm::distance(tpos, pos) > 5)
			continue;

		if (pos.y > tpos.y && pos.y < tpos.y + 1) {
			float frac = pos.y - tpos.y;

			float radius = (1 - frac) * 1.5;

			glm::vec3 aux = glm::vec3(tpos.x, pos.y, tpos.z);
			if (glm::distance(aux, pos) < radius + 0.2) {
				glm::vec3 aux2 = pos - aux;
				aux2 = glm::normalize(aux2);

				pos = aux + aux2 * (radius + 0.2f);
				speed.y /= 2;

				return i;
			}
		}

		if (pos.y > tpos.y + 0.5 && pos.y < tpos.y + 2) {
			float frac = (pos.y - tpos.y - 0.5) / 1.5;

			float radius = (1 - frac) * 1.2;

			glm::vec3 aux = glm::vec3(tpos.x, pos.y, tpos.z);
			if (glm::distance(aux, pos) < radius + 0.2) {
				glm::vec3 aux2 = pos - aux;
				aux2 = glm::normalize(aux2);

				pos = aux + aux2 * (radius + 0.2f);
				speed.y /= 1.25;

				return i;
			}
		}

		if (pos.y > tpos.y + 1.4 && pos.y < tpos.y + 3) {
			float frac = (pos.y - tpos.y - 1.4) / 1.6;

			float radius = (1 - frac) * 0.8;

			glm::vec3 aux = glm::vec3(tpos.x, pos.y, tpos.z);
			if (glm::distance(aux, pos) < radius + 0.2) {
				glm::vec3 aux2 = pos - aux;
				aux2 = glm::normalize(aux2);

				pos = aux + aux2 * (radius + 0.2f);
				speed.y /= 1.1;

				return i;
			}
		}

		if (pos.y < tpos.y && glm::distance(glm::vec3(pos.x, 0, pos.z), glm::vec3(tpos.x, 0, tpos.z)) < 0.4) {
			glm::vec2 aux = glm::vec2(pos.x - tpos.x, pos.z - tpos.z);
			aux = glm::normalize(aux);

			pos.x = tpos.x + aux.x * 0.4;
			pos.z = tpos.z + aux.y * 0.4;

			speed = glm::vec3(0, speed.y, 0);

			return i;
		}
	}
	return -1;
}

void Drone::updatePos(float deltaTimeSeconds, bool isMovingOX, bool isMovingOZ) {
	speed.y -= 1.5 * deltaTimeSeconds;
	if (speed.x != 0 || speed.z != 0)
		speed -= glm::normalize(glm::vec3(speed.x, 0, speed.z)) * deltaTimeSeconds;
	if (abs(speed.x) < EPSILON && !isMovingOX)
		speed.x = 0;
	if (abs(speed.z) < EPSILON && !isMovingOZ)
		speed.z = 0;

	pos += speed * deltaTimeSeconds;

	intersectsTree();
	intersectsPlatform();

	float terrainY = noise(glm::vec2(pos.x, pos.z));

	if (pos.y < terrainY) {
		pos.y = terrainY;
		speed.y = 0;
	}
	if (pos.y > 40) {
		pos.y = 40;
		speed.y = 0;
	}
	if (pos.x < -100) {
		pos.x = -100;
		speed.x = 0;
	}
	if (pos.x > 100) {
		pos.x = 100;
		speed.x = 0;
	}
	if (pos.z < -100) {
		pos.z = -100;
		speed.z = 0;
	}
	if (pos.z > 100) {
		pos.z = 100;
		speed.z = 0;
	}
}

void Drone::accelerate(float deltaTimeSeconds) {
	speed += glm::normalize(glm::vec3(up.x, up.y / 2, up.z)) * 2.5f * deltaTimeSeconds;

	if (pos.y < 0)
		pos.y = 0;
}

void Drone::rotateOY(float angle) {
	glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(front, 1);
	front = glm::normalize(glm::vec3(newVector));

	newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
	right = glm::normalize(glm::vec3(newVector));

	up = glm::cross(right, front);
	up = glm::normalize(up);
}

void Drone::computeUp() {
	glm::mat4 rotation1 = glm::rotate(glm::mat4(1), angleRight, front);
	glm::mat4 rotation2 = glm::rotate(glm::mat4(1), angleFront, right);

	up = glm::vec3(rotation1 * rotation2 * glm::vec4(0, 1, 0, 1));

}