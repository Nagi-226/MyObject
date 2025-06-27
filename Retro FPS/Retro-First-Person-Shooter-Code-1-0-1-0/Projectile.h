#pragma once
#include "SDL2/SDL.h"
#include "Sprite.h"
#include "TextureLoader.h"
#include "Level.h"
class UnitPlayer;
class UnitEnemy;



class Projectile : public Sprite
{
public:
	Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal, bool setShotFromPlayer);
	void update(float dT, std::unique_ptr<UnitPlayer>& unitPlayer, std::vector<std::shared_ptr<UnitEnemy>>& listUnitEnemies);
	bool getCollisionOccurred();


private:
	void checkCollisions(std::unique_ptr<UnitPlayer>& unitPlayer, std::vector<std::shared_ptr<UnitEnemy>>& listUnitEnemies);


	Vector2D directionNormal;

	const float speed = 20.0f;

	const int damage = 1;

	const bool shotFromPlayer;

	bool collisionOccurred = false;
};