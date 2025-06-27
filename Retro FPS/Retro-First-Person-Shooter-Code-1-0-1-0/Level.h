#pragma once
#include <algorithm>
#include <memory>
#include <vector>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "Pickup.h"
#include "Health.h"
#include "Coin.h"
#include "Ammo.h"
#include "UpgradeHealth.h"
#include "UpgradeAmmo.h"
#include "UpgradeWeaponSpeed.h"
class UnitEnemy;



class Level
{
public:
	static bool isBlockAtPos(int x, int y);
	
	static void setupAllEnemiesAndPickups(SDL_Renderer* renderer, Vector2D& posStart, Vector2D& posFinish, 
		std::vector<std::shared_ptr<UnitEnemy>>& listUnitEnemies, std::vector<std::shared_ptr<Pickup>>& listPickups);


	static const char* levelData;

	static const int levelWidth;
	static const size_t levelSize;


	static const char symbolWall = 'X';
	//static const char symbolEmpty = ' ';

	static const char symbolHealth = 'H';
	static const char symbolCoin = 'C';
	static const char symbolAmmo = 'A';

	static const char symbolUpgradeHealth = 'h';
	static const char symbolUpgradeAmmo = 'a';
	static const char symbolUpgradeWeaponSpeed = 'w';

	static const char symbolStart = 'S';
	static const char symbolFinish = 'F';

	static const char symbolEnemySmall = '0';
	static const char symbolEnemyMedium = '1';
	static const char symbolEnemyLarge = '2';
};