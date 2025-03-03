#pragma once

#include <vector>
#include "utils/glm_utils.h"

#define EPSILON 0.001

class Drone
{
	public:
		Drone();
		~Drone();

		int intersectsTree();
		int intersectsPlatform();
		void accelerate(float deltaTimeSeconds);
		void updatePos(float deltaTimeSeconds, bool isMovingOX, bool isMovingOZ);
		void computeUp();
		void rotateOY(float angle);

		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 front = glm::vec3(1, 0, 0);
		glm::vec3 right = glm::vec3(0, 0, -1);
		glm::vec3 pos = glm::vec3(0, 0, 0);
		glm::vec3 speed = glm::vec3(0, 0, 0);

		float angleFront = 0, angleRight = 0;
		float angle = 0;
		bool packageAtached = false;
		std::vector<glm::vec3>* treesPos;
		std::vector<glm::vec3>* platPos;
};