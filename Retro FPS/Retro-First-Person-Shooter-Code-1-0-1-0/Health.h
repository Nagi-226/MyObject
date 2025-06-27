#pragma once
#include "Pickup.h"



class Health : public Pickup
{
public:
	Health(SDL_Renderer* renderer, Vector2D setPos);
	bool addPickupToPlayer(std::unique_ptr<UnitPlayer>& unitPlayer);
};