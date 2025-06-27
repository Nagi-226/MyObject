#include "Coin.h"
#include "UnitPlayer.h"



Coin::Coin(SDL_Renderer* renderer, Vector2D setPos) :
	Pickup(renderer, setPos, "Coin.bmp", "Coin.ogg", true) {

}



bool Coin::addPickupToPlayer(std::unique_ptr<UnitPlayer>& unitPlayer) {
	if (unitPlayer != nullptr) {
		unitPlayer->addCoin();
		return true;
	}

	return false;
}