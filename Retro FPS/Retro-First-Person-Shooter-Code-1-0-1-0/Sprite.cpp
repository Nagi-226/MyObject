#include "Sprite.h"
#include "UnitPlayer.h"
#include "Game.h"



Sprite::Sprite(SDL_Renderer* renderer, Vector2D setPos, std::string filenameForTexture, float setOverlapRadius, float setDrawScale) :
	pos(setPos), texture(TextureLoader::loadTexture(renderer, filenameForTexture)),
	overlapRadius(setOverlapRadius), drawScale(setDrawScale) {

}



void Sprite::draw(SDL_Renderer* renderer, int worldWidth, int worldHeight, std::unique_ptr<UnitPlayer>& unitPlayer, 
    float fovRad, float listDepthDraw[]) {
    //Draw the image
    //Assume that it's 16x16 pixels for simplicity.
    //Start by finding the correct scale and position to draw the image on the screen.
    //Then draw the image vertical line by vertical line.
    if (texture != nullptr && unitPlayer != nullptr) {
        //Set the color to red if needed (eg. an enemy unit was recently hurt).
        if (drawRed)
            SDL_SetTextureColorMod(texture, 255, 0, 0);
        else
            SDL_SetTextureColorMod(texture, 255, 255, 255);

        //The angle between the direction the player is looking and the sprite being drawn.
        float angleDelta = (Vector2D(unitPlayer->getAngle())).angleBetween(pos - unitPlayer->getPos());

        //The distance between the sprite and player.
        float distance = (pos - unitPlayer->getPos()).magnitude();
        //If it's 0 then make it very small so that the image can still be drawn really large
        //(by avoiding divide by 0).
        if (distance == 0.0f)
            distance = 0.0001f;

        //The number of pixels that each angle (in radians) represents on the screen.
        float screenPixelsPerRad = worldWidth / fovRad;

        //The size that the sprite's image will be drawn on the screen.
        float sizeDraw = 2.0f * drawScale * (atan(0.5f / distance) * screenPixelsPerRad);

        //The center position where the image will be drawn on the screen.
        float xCenter = worldWidth / 2.0f + screenPixelsPerRad * angleDelta;
        float yCenter = worldHeight / 2.0f;

        //The left and top position where the image will be drawn on the screen.
        int xLeft = (int)round(xCenter - sizeDraw / 2);
        int yTop = (int)round(yCenter - sizeDraw / 2);

        //The width of the vertical line on the image.
        //Note 16 - 1 because the texture's coordinates start at 0 for the first pixel not 1.
        //They go up to 15 not 16, for a total of 16 positions.
        int widthPerLine = std::max((int)round((float)(16 - 1) / sizeDraw), 1);

        //Draw the image on the screen vertical line by vertial line.
        int xMax = (int)round(sizeDraw);
        for (int x = 0; x < xMax; x++) {
            int xDest = x + xLeft;
            //Ensure that the line will actually be on the screen.
            //Note part of the image could be on the screen and part off the screen.
            if (xDest > -1 && xDest < worldWidth) {
                //Check the depth/distance that has currently been drawn for this vertical line.
                //Only draw this vertical line if it's distance to the player is close or equal to what was 
                //drawn previously.  Note the equal sign is there because under certain conditions there was 
                //an issue without it.
                if (listDepthDraw[xDest] >= distance) {
                    listDepthDraw[xDest] = distance;

                    //The position of the vertical line on the image to be used for drawing.
                    SDL_Rect rectSrc = {
                    (int)round((float)x / (xMax - 1) * (float)(16 - 1)),
                    0,
                    widthPerLine,
                    16 };

                    //The position of the vertical line on the screen to be drawn.
                    SDL_Rect rectDest = {
                    xDest,
                    yTop,
                    1,
                    (int)round(sizeDraw) };

                    SDL_RenderCopy(renderer, texture, &rectSrc, &rectDest);
                }
            }
        }
    }
}



bool Sprite::checkOverlap(Sprite* other) {
    if (other != nullptr)
	    return sqrt(pow(other->pos.x - pos.x, 2.0f) + pow(other->pos.y - pos.y, 2.0f)) <= (other->overlapRadius + overlapRadius);

    return false;
}



Vector2D Sprite::getPos() {
    return pos;
}