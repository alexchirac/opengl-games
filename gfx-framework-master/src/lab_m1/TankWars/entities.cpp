#include "lab_m1/TankWars/entities.h"

#include <vector>
#include <iostream>
#include <math.h>

entities::Tank::Tank(float posx, float turretAngle, std::vector<float>& ys, float rectsPerUnit, float maxX)
{
	this->posx = posx;
	this->ys = &ys;
	this->rectsPerUnit = rectsPerUnit;
	this->maxX = maxX;
	this->turretAngle = turretAngle;
	calculateYAngleAndCenter();
}

void entities::Tank::calculateYAngleAndCenter()
{
	int i = floor(posx * rectsPerUnit);

	float prevx = (float)i / rectsPerUnit;
	float nextx = (float)(i + 1) / rectsPerUnit;

	float prevy = (*ys)[i];
	float nexty = (*ys)[i + 1];

	this->posy = prevy + (this->posx - prevx) / (nextx - prevx) * (nexty - prevy);
	angle = atan2(nexty - prevy, nextx - prevx);

	this->centerx = posx - TANK_HEIGHT * sin(angle);
	this->centery = posy + TANK_HEIGHT * cos(angle);
}

void entities::Tank::move(float dx)
{
	this->posx += dx;
	if (this->posx > maxX)
		this->posx = maxX;
	if (this->posx < 0)
		this->posx = 0;
	calculateYAngleAndCenter();
}

void entities::Tank::moveTurret(float angle)
{
	this->turretAngle += angle;
}

void entities::Tank::loseHealth()
{
	this->health -= 10;
	if (this->health < 0)
		this->health = 0;
}

entities::Projectile::Projectile(float posx, float posy, float vx, float vy, std::vector<float>& ys, float rectsPerUnit, float maxX) 
	: posx(posx), posy(posy), vx(vx), vy(vy), ys(&ys), rectsPerUnit(rectsPerUnit), maxX(maxX) {}

entities::Projectile::~Projectile() {}

void entities::Projectile::update(float deltaTime)
{
	this->posx += this->vx * 2 * deltaTime;
	this->posy += this->vy * 2 * deltaTime;
	this->vy -= GRAVITY * deltaTime;
}

float entities::Projectile::getProjectionY()
{

	int i = floor(posx * rectsPerUnit);

	float prevx = (float)i / rectsPerUnit;
	float nextx = (float)(i + 1) / rectsPerUnit;

	float prevy = (*ys)[i];
	float nexty = (*ys)[i + 1];

	return prevy + (this->posx - prevx) / (nextx - prevx) * (nexty - prevy);

}
