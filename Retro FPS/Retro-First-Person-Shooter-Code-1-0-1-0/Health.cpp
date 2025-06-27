#include "Health.h"
#include "UnitPlayer.h"



Health::Health(SDL_Renderer* renderer, Vector2D setPos) :
	Pickup(renderer, setPos, "Heart.bmp", "Health.ogg", true) {

}



bool Health::addPickupToPlayer(std::unique_ptr<UnitPlayer>& unitPlayer) {
	if (unitPlayer != nullptr && unitPlayer->isHealthFull() == false) {
		unitPlayer->addHealth(5);
		return true;
	}

	return false;
}