#pragma once
#include "Pickup.h"



class UpgradeWeaponSpeed : public Pickup
{
public:
	UpgradeWeaponSpeed(SDL_Renderer* renderer, Vector2D setPos);
	bool addPickupToPlayer(std::unique_ptr<UnitPlayer>& unitPlayer);
};