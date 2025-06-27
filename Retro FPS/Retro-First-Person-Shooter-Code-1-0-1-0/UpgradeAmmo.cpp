#include "UpgradeAmmo.h"
#include "UnitPlayer.h"



UpgradeAmmo::UpgradeAmmo(SDL_Renderer* renderer, Vector2D setPos) :
	Pickup(renderer, setPos, "Upgrade Ammo 10.bmp", "Upgrade.ogg", false) {

}



bool UpgradeAmmo::addPickupToPlayer(std::unique_ptr<UnitPlayer>& unitPlayer) {
	if (unitPlayer != nullptr && unitPlayer->buyUpgradeAmmoMax())
		return true;

	return false;
}