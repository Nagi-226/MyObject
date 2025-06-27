#pragma once
#include <string>
#include <memory>
#include "SDL2/SDL.h"
#include "TextureLoader.h"
#include "Vector2D.h"
class UnitPlayer;
class Game;



class Sprite
{
public:
	Sprite(SDL_Renderer* renderer, Vector2D setPos, std::string filenameForTexture,
		float setOverlapRadius = 0.5f, float setDrawScale = 1.0f);
	void draw(SDL_Renderer* renderer, int worldWidth, int worldHeight, std::unique_ptr<UnitPlayer>& unitPlayer, 
		float fovRad, float listDepthDraw[]);
	bool checkOverlap(Sprite* other);
	Vector2D getPos();


protected:
	Vector2D pos;
	bool drawRed = false;


private:
	SDL_Texture* texture = nullptr;
	float overlapRadius, drawScale;
};