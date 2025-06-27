#pragma once
#include "SoundLoader.h"
#include "Sprite.h"



class Pickup : public Sprite
{
public:
	Pickup(SDL_Renderer* renderer, Vector2D setPos, std::string filenameForTexture, std::string filenameForSound, 
		bool setConsumable);
	void update(std::unique_ptr<UnitPlayer>& unitPlayer);
	bool getConsumed();


protected:
	virtual bool addPickupToPlayer(std::unique_ptr<UnitPlayer>& unitPlayer);


private:
	const bool consumable;
	bool consumed = false, justOverlaped = false;

	Mix_Chunk* mix_ChunkPickup = nullptr;
};