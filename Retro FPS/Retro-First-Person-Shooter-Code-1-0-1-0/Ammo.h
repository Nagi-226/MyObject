#pragma once
#include "Pickup.h"



class Ammo : public Pickup
{
public:
	Ammo(SDL_Renderer* renderer, Vector2D setPos);
	bool addPickupToPlayer(std::unique_ptr<UnitPlayer>& unitPlayer);
};