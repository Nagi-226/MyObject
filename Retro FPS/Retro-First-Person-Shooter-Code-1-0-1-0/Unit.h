#pragma once
#include "SDL2/SDL.h"
#include "Sprite.h"
#include "Weapon.h"



class Unit : public Sprite
{
public:
	Unit(SDL_Renderer* renderer, Vector2D setPos, std::string filenameForTexture, int setHealthMax, Weapon setWeapon);
	void update(float dT);
	void draw(SDL_Renderer* renderer);
	bool isAlive();
	void removeHealth(int amount);


protected:
	int healthMax;
	int healthCurrent = healthMax;

	Weapon weapon;
};