#pragma once
#include "Unit.h"
#include "Vector2D.h"
#include "MathAddon.h"



class UnitPlayer : public Unit
{
public:
	UnitPlayer(SDL_Renderer* renderer, Vector2D setPos);
	void update(float dT);
	void shootProjectile(SDL_Renderer* renderer, std::vector<std::shared_ptr<Projectile>>& listProjectiles);
	void setDirectionForward(int setDirectionForward);
	void setDirectionRight(int setDirectionRight);
	void setAmountTurn(float setDirectionTurn);
	float getAngle();
	std::string getHealthString();
	bool isHealthFull();
	void addHealth(int amount);
	void addCoin();
	int getCountCoins();
	bool isAmmoFull();
	void addAmmo(int amount);
	std::string computeAmmoString();

	bool buyUpgradeHealthMax();
	bool buyUpgradeAmmoMax();
	bool buyUpgradeWeaponSpeed();


private:
	int directionForward = 0, directionRight = 0;
	float amountTurn = 0.0f;

	float angle;
	const float speedMove, speedTurn;


	int countCoins = 0;

	int countHealthMaxUpgrades = 0, countAmmoMaxUpgrades = 0, countWeaponSpeedUpgrades = 0;
};