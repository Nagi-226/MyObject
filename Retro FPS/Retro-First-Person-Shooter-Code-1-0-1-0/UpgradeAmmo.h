#pragma once
#include "Pickup.h"



class UpgradeAmmo : public Pickup
{
public:
	UpgradeAmmo(SDL_Renderer* renderer, Vector2D setPos);
	bool addPickupToPlayer(std::unique_ptr<UnitPlayer>& unitPlayer);
};