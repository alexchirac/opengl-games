#pragma once

#include <vector>
#define GRAVITY 500
#define TANK_HEIGHT 20
#define TURRET_LEN 40
#define EPSILON 1


namespace entities
{
	class Tank
	{
	public:
		Tank(float posx, float turretAngle, std::vector<float>& ys, float rectsPerUnit, float maxX);
		~Tank();

		void calculateYAngleAndCenter();
		void move(float dx);
		void moveTurret(float angle);
		void loseHealth();

		float health = 100;
		float maxHealth = 100;
		float posx, posy;
		float centerx, centery;
		float angle;
		float maxX;
		float turretAngle;

		std::vector<float>* ys;
		float rectsPerUnit;
	};

	class Projectile
	{
	public:
		Projectile(float posx, float posy, float vx, float vy, std::vector<float>& ys, float rectsPerUnit, float maxX);
		~Projectile();

		void update(float deltaTime);
		float getProjectionY();

		float posx;
		float posy;
		float vx;
		float vy;
		float maxX;

		std::vector<float>* ys;
		float rectsPerUnit;
	};
}
