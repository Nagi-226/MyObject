#include "Weapon.h"


bool Weapon::soundLoaded = false;
Mix_Chunk* Weapon::mix_ChunkWeaponShoot = nullptr;




Weapon::Weapon(int setAmmo, int setRoundsPerSecond) : ammo(setAmmo), 
roundsPerSecond(setRoundsPerSecond), cooldownTimer(setRoundsPerSecond > 0 ? (1.0f / setRoundsPerSecond) : (1.0f / 2)) {
	if (soundLoaded == false) {
		mix_ChunkWeaponShoot = SoundLoader::loadSound("Energy Orb.ogg");
		soundLoaded = (mix_ChunkWeaponShoot != nullptr);
	}
}



void Weapon::update(float dT) {
	//Update weapon cooldown.
	cooldownTimer.countDown(dT * (ammo == 0 ? 0.25f : 1.0f));
}



void Weapon::shootProjectile(SDL_Renderer* renderer, Vector2D start, Vector2D directionNormal, 
	std::vector<std::shared_ptr<Projectile>>& listProjectiles, bool setShotFromPlayer, 
	float angleSoundDeg, float distanceSound) {
	//Ensure the weapon is ready to shoot.
	if (cooldownTimer.timeSIsZero()) {
		//The projectile will move from the start position in the direction of directionNormal.
		//Add the projectile to the list.
		listProjectiles.push_back(std::make_unique<Projectile>(renderer, start, directionNormal, setShotFromPlayer));

		//Play sound.
		if (mix_ChunkWeaponShoot != nullptr) {
			int channelSelected = Mix_PlayChannel(-1, mix_ChunkWeaponShoot, 0);
			//If it wasn't shot from the player then adjust it's volume based on it's position and angle relative to the player.
			if (setShotFromPlayer == false && channelSelected > -1) {
				float fDistanceSound = sqrt(distanceSound / 50.0f);
				if (fDistanceSound < 0.0f)
					fDistanceSound = 0.0f;
				if (fDistanceSound > 0.7f)
					fDistanceSound = 0.7f;

				Mix_SetPosition(channelSelected, (int)angleSoundDeg, (int)(fDistanceSound * 255));
			}
		}

		cooldownTimer.resetToMax();

		if (ammo > 0)
			ammo--;
	}
}



bool Weapon::isAmmoFull() {
	return (ammo == ammoMax);
}


void Weapon::addAmmo(int amount) {
	if (amount > 0)
		ammo += amount;

	if (ammo > ammoMax)
		ammo = ammoMax;
}


std::string Weapon::computeAmmoString() {
	if (ammo > 0) {
		if (ammo == ammoMax)
			return "Max";
		else
			return std::to_string(ammo);
	}
	else if (ammo < 0)
		return "Inf";

	return "Low";
}



void Weapon::upgradeAmmoMax() {
	ammoMax += 10;
}



void Weapon::upgradeWeaponSpeed() {
	roundsPerSecond += 2;
	if (roundsPerSecond > 0)
		cooldownTimer = Timer(1.0f / roundsPerSecond);
}