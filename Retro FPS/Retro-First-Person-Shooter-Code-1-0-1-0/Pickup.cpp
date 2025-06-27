#include "Pickup.h"
#include "UnitPlayer.h"



Pickup::Pickup(SDL_Renderer* renderer, Vector2D setPos, std::string filenameForTexture, std::string filenameForSound,
	bool setConsumable) :
	Sprite(renderer, setPos, filenameForTexture), mix_ChunkPickup(SoundLoader::loadSound(filenameForSound)),
	consumable(setConsumable) {

}



void Pickup::update(std::unique_ptr<UnitPlayer>& unitPlayer) {
	if (unitPlayer != nullptr) {
		if (justOverlaped == false && checkOverlap(unitPlayer.get()) && addPickupToPlayer(unitPlayer)) {
			justOverlaped = true;

			//Play sound.
			if (mix_ChunkPickup != nullptr)
				Mix_PlayChannel(-1, mix_ChunkPickup, 0);

			if (consumable)
				consumed = true;
		}
		else if (justOverlaped && checkOverlap(unitPlayer.get()) == false) {
			justOverlaped = false;
		}
	}
}



bool Pickup::getConsumed() {
	return consumed;
}



bool Pickup::addPickupToPlayer(std::unique_ptr<UnitPlayer>& unitPlayer) {
	return true;
}