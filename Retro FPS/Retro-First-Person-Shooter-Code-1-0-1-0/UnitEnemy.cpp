#include "UnitEnemy.h"
#include "Game.h"


const std::vector<UnitEnemy::TemplateData> UnitEnemy::listTemplateData = {
	{"Alien Small.bmp", 1, Weapon(-1, 1), false },
	{"Alien Medium.bmp", 3, Weapon(15, 4), true },
	{"Alien Large.bmp", 6, Weapon(30, 8), true }
};




UnitEnemy::UnitEnemy(SDL_Renderer* renderer, Vector2D setPos, TemplateData unitEnemyTemplateData) :
	Unit(renderer, setPos,
		unitEnemyTemplateData.filenameForTexture,
		unitEnemyTemplateData.healthMax,
		unitEnemyTemplateData.weapon),
	justHurtTimer(0.25f), playerBecameVisibleTimer(2.0f),
	hasChanceToDropPickup(unitEnemyTemplateData.hasChanceToDropPickup) {

}



void UnitEnemy::addUnitEnemyToListUsingTemplate(SDL_Renderer* renderer, Vector2D setPos, int templateID,
	std::vector<std::shared_ptr<UnitEnemy>>& listUnitEnemies) {
	if (templateID > -1 && templateID < listTemplateData.size()) {
		listUnitEnemies.push_back(std::make_shared<UnitEnemy>(renderer, setPos, listTemplateData[templateID]));
	}
}



void UnitEnemy::update(float dT, SDL_Renderer* renderer, Game& game, std::unique_ptr<UnitPlayer>& unitPlayer, 
	std::vector<std::shared_ptr<Projectile>>& listProjectiles) {

	Unit::update(dT);


	justHurtTimer.countDown(dT);
	drawRed = (justHurtTimer.timeSIsZero() == false);


	//Check and handle player visibility.
	bool unitPlayerVisible = checkIfUnitPlayerVisible(game, unitPlayer);

	if (unitPlayerVisible)
		playerBecameVisibleTimer.countUp(dT);
	else
		playerBecameVisibleTimer.countDown(dT);


	//If unit player has been visible for long enough then shoot it.
	if (playerBecameVisibleTimer.timeSIsGreaterThanOrEqualTo(0.5f)
		&& unitPlayerVisible && unitPlayer != nullptr) {
		Vector2D directionPlayer(unitPlayer->getPos() - pos);
		Vector2D directionNormal(Vector2D(directionPlayer).normalize());

		float angleSoundDeg = MathAddon::angleRadToDeg(directionNormal.angleBetween(Vector2D(unitPlayer->getAngle())));

		weapon.shootProjectile(renderer, pos, directionNormal, listProjectiles, false, angleSoundDeg, directionPlayer.magnitude());
	}
}



bool UnitEnemy::checkIfUnitPlayerVisible(Game& game, std::unique_ptr<UnitPlayer>& unitPlayer) {
	if (unitPlayer != nullptr) {
		Vector2D normal((unitPlayer->getPos() - pos).normalize());

		float distance = game.raycast(pos, normal, true).first;
		if (distance > 0.0f)
			return true;
	}

	return false;
}



void UnitEnemy::removeHealth(int damage) {
	Unit::removeHealth(damage);

	if (damage > 0)
		justHurtTimer.resetToMax();
}



bool UnitEnemy::getHasChanceToDropPickup() {
	return hasChanceToDropPickup;
}