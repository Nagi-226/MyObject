#include "Game.h"


const float Game::fovRad = MathAddon::angleDegToRad(60.0f);




Game::Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight) :
    gameModeCurrent(Mode::instructions) {
    //Run the game.
    if (window != nullptr && renderer != nullptr) {
        //Initialize a texture that will be used to draw the world.  It will be a scaled amount of the screen.
        textureScreen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, worldWidth, worldHeight);
        SDL_SetTextureBlendMode(textureScreen, SDL_BLENDMODE_BLEND);

        //Load some needed textures.
        textureFont = TextureLoader::loadTexture(renderer, "Font.bmp");
        textureCrosshair = TextureLoader::loadTexture(renderer, "Crosshair.bmp");
        //The following are for the in game overlay.
        textureHeart = TextureLoader::loadTexture(renderer, "Heart.bmp");
        textureAmmo = TextureLoader::loadTexture(renderer, "Battery.bmp");
        textureCoin = TextureLoader::loadTexture(renderer, "Coin.bmp");

        //Load the level data.
        Vector2D posStart, posFinish;
        Level::setupAllEnemiesAndPickups(renderer, posStart, posFinish, listUnitEnemies, listPickups);
        unitPlayer = std::make_unique<UnitPlayer>(renderer, posStart);
        spriteFlag = std::make_shared<Sprite>(renderer, posFinish, "Flag.bmp");

        listVisibleCells.assign(Level::levelSize, false);


        //Store the current times for the clock.
        auto time1 = std::chrono::system_clock::now();
        auto time2 = std::chrono::system_clock::now();

        //The amount of time for each frame (60 fps).
        const float dT = 1.0f / 60.0f;


        //Start the game loop and run until it's time to stop.
        bool running = true;
        while (running) {
            //Determine how much time has elapsed since the last frame.
            time2 = std::chrono::system_clock::now();
            std::chrono::duration<float> timeDelta = time2 - time1;
            float timeDeltaFloat = timeDelta.count();

            //If enough time has passed then do everything required to generate the next frame.
            if (timeDeltaFloat >= dT) {
                //Store the new time for the next frame.
                time1 = time2;

                //Process events.
                processEvents(running, renderer, window, windowWidth, windowHeight);

                //Update.  Only do so if the game mode is playing.
                if (gameModeCurrent == Mode::playing)
                    update(timeDeltaFloat, renderer);

                //Draw.
                std::string framerate = (timeDeltaFloat > 0.0f ? std::to_string((int)round(1.0f / timeDeltaFloat)) : "Inf");
                draw(renderer, framerate);
            }
        }
    }
}



Game::~Game() {
    //Clean up.
    if (textureScreen != nullptr) {
        SDL_DestroyTexture(textureScreen);
        textureScreen = nullptr;
    }
    TextureLoader::deallocateTextures();
    SoundLoader::deallocateSounds();
}



void Game::processEvents(bool& running, SDL_Renderer* renderer, SDL_Window* window, int windowWidth, int windowHeight) {
    bool mouseDownThisFrame = false;

    //Process events.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            //If any mouse button is pressed then hide the instructions if they're visible.
            if (gameModeCurrent == Mode::instructions) {
                gameModeCurrent = Mode::playing;
            }
            else {
                //Otherwise store the mouse event information.
                mouseDownThisFrame = (mouseDownStatus == 0);
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouseDownStatus = SDL_BUTTON_LEFT;
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    mouseDownStatus = SDL_BUTTON_RIGHT;
            }

            break;
        case SDL_MOUSEBUTTONUP:
            mouseDownStatus = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                //Quit the game.
            case SDL_SCANCODE_ESCAPE:
                running = false;
                break;
            }
            break;
        }
    }

    //Get the relative movement of the mouse.
    int mouseXOffset;
    SDL_GetRelativeMouseState(&mouseXOffset, NULL);

    if (unitPlayer != nullptr && gameModeCurrent == Mode::playing) {
        //A more responsive/faster way to check keypresses for the unit movement.
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        //Make the unit move if needed.
        if (keyboardState[SDL_SCANCODE_A])
            unitPlayer->setDirectionRight(-1);
        if (keyboardState[SDL_SCANCODE_D])
            unitPlayer->setDirectionRight(1);
        if (keyboardState[SDL_SCANCODE_W])
            unitPlayer->setDirectionForward(1);
        if (keyboardState[SDL_SCANCODE_S])
            unitPlayer->setDirectionForward(-1);

        //Make the unit turn if needed.
        if (mouseXOffset != 0)
            unitPlayer->setAmountTurn((float)mouseXOffset / windowWidth);

        //Make the unit shoot if needed.
        if (mouseDownStatus == SDL_BUTTON_LEFT)
            unitPlayer->shootProjectile(renderer, listProjectiles);
    }
}



void Game::update(float dT, SDL_Renderer* renderer) {
    //Update.
    if (unitPlayer != nullptr) {
        unitPlayer->update(dT);
        if (unitPlayer->isAlive() == false)
            gameModeCurrent = Mode::defeat;
        else if (spriteFlag != nullptr && spriteFlag->checkOverlap(unitPlayer.get()))
            gameModeCurrent = Mode::victory;
    }

    //Update the enemy units.
    for (int count = 0; count < listUnitEnemies.size(); count++) {
        auto& unitEnemySelected = listUnitEnemies[count];
        if (unitEnemySelected != nullptr) {
            unitEnemySelected->update(dT, renderer, *this, unitPlayer, listProjectiles);
            if (unitEnemySelected->isAlive() == false) {
                if (unitEnemySelected->getHasChanceToDropPickup())
                    addRandomPickup(renderer, unitEnemySelected->getPos());

                listUnitEnemies.erase(listUnitEnemies.begin() + count);
                count--;
            }
        }
    }

    //Update the pickups.
    for (int count = 0; count < listPickups.size(); count++) {
        auto& pickupSelected = listPickups.at(count);
        if (pickupSelected != nullptr) {
            pickupSelected->update(unitPlayer);
            if (pickupSelected->getConsumed()) {
                listPickups.erase(listPickups.begin() + count);
                count--;
            }
        }
    }

    //Update the projectiles.
    for (int count = 0; count < listProjectiles.size(); count++) {
        auto& projectileSelected = listProjectiles.at(count);
        if (projectileSelected != nullptr) {
            projectileSelected->update(dT, unitPlayer, listUnitEnemies);
            if (projectileSelected->getCollisionOccurred()) {
                listProjectiles.erase(listProjectiles.begin() + count);
                count--;
            }
        }
    }
}



void Game::draw(SDL_Renderer* renderer, std::string framerate) {
    //Clear the screen.
    //Set the draw color to dark blue (it will be the color of the ceiling)
    SDL_SetRenderDrawColor(renderer, 0, 0, 92, 255);
    SDL_RenderClear(renderer);


    //Switch the render target to textureScreen and clear it.
    SDL_SetRenderTarget(renderer, textureScreen);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    //Draw the entire world.
    drawWorld(renderer);

    //Draw the overlays.
    switch (gameModeCurrent) {
    case Mode::instructions:
        drawOverlayInstructions(renderer);
        break;
    case Mode::playing:
        drawOverlayPlaying(renderer);
        break;
    case Mode::victory:
        drawOverlayVictory(renderer);
        break;
    case Mode::defeat:
        drawOverlayDefeat(renderer);
        break;
    }

    //Draw the framerate
    //drawText(renderer, 2, 2, 1, framerate);


    //Set the render target back to the window.
    SDL_SetRenderTarget(renderer, NULL);
    //Draw the texture.
    SDL_RenderCopy(renderer, textureScreen, NULL, NULL);


    //Send the image to the window.
    SDL_RenderPresent(renderer);
}



void Game::drawWorld(SDL_Renderer* renderer) {
    //Set the draw color to light blue (for the floor)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255 - 92, 255);
    //Draw a rectangle that covers the bottom half of the screen.
    //Note that the +1 to the height is incase worldHeight / 2 has round off error.
    SDL_Rect rectFloor = { 0, worldHeight / 2, worldWidth, (worldHeight / 2 + 1) };
    SDL_RenderFillRect(renderer, &rectFloor);


    drawWalls(renderer);
    addAllSpritesToDrawList(renderer);
    //Draw a number on the screen for the number of sprites that need to be drawn.
    //drawText(renderer, 2, 14, 1, std::to_string(listSpritesToDraw.size()));
    sortAndDrawListSpritesToDraw(renderer);
}



void Game::drawOverlayInstructions(SDL_Renderer* renderer) {
    //Draw a transparent black overlay that covers the whole screen.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect rectBackground{ 0,0, worldWidth, worldHeight };
    SDL_RenderFillRect(renderer, &rectBackground);

    //Draw the text.
    drawText(renderer, 55, 35, 1, "W, A, S, D:  Movement");
    drawText(renderer, 55, 50, 1, "Move Mouse:  Turn");
    drawText(renderer, 55, 65, 1, "Left Click:  Hold to Shoot");
    drawText(renderer, 55, 80, 1, "ESC:         Quit");
    drawText(renderer, 72, 100, 1, "-Click to Start-");
}


void Game::drawOverlayPlaying(SDL_Renderer* renderer) {
    if (unitPlayer != nullptr) {
        //Draw a transparent black overlay that covers a bit of the bottom left of the screen.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect rectBackground{ 0, worldHeight - 14, 110, 14 };
        SDL_RenderFillRect(renderer, &rectBackground);

        //Draw the heart image and the players amount of health.
        SDL_Rect rectHeart{ 0, worldHeight - 19, 16, 16 };
        SDL_RenderCopy(renderer, textureHeart, NULL, &rectHeart);
        drawText(renderer, 16, worldHeight - 10, 1, unitPlayer->getHealthString());

        //Draw the battery/ammo image and the players amount of ammo.
        SDL_Rect rectAmmo{ 40, worldHeight - 19, 16, 16 };
        SDL_RenderCopy(renderer, textureAmmo, NULL, &rectAmmo);
        drawText(renderer, 54, worldHeight - 10, 1, unitPlayer->computeAmmoString());

        //Draw the coin image and the players amount of coins.
        SDL_Rect rectCoin{ 80, worldHeight - 19, 16, 16 };
        SDL_RenderCopy(renderer, textureCoin, NULL, &rectCoin);
        drawText(renderer, 96, worldHeight - 10, 1, std::to_string(unitPlayer->getCountCoins()));

        //Draw the crosshair.  Assume that it's 16x16 pixels for simplicity.
        SDL_Rect rectCrosshair{ (worldWidth - 16) / 2, (worldHeight - 16) / 2, 16, 16 };
        SDL_RenderCopy(renderer, textureCrosshair, NULL, &rectCrosshair);
    }
}


void Game::drawOverlayVictory(SDL_Renderer* renderer) {
    //Draw a transparent green overlay that covers the whole screen.
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 160);
    SDL_Rect rectBackground{ 0,0, worldWidth, worldHeight };
    SDL_RenderFillRect(renderer, &rectBackground);

    //Draw the text.
    drawText(renderer, 53, 53, 3, "You Won!");
    drawText(renderer, 63, 89, 1, "-Press ESC to Quit-");
}


void Game::drawOverlayDefeat(SDL_Renderer* renderer) {
    //Draw a transparent red overlay that covers the whole screen.
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 192);
    SDL_Rect rectBackground{ 0,0, worldWidth, worldHeight };
    SDL_RenderFillRect(renderer, &rectBackground);

    //Draw the text.
    drawText(renderer, 44, 53, 3, "You Lost!");
    drawText(renderer, 63, 89, 1, "-Press ESC to Quit-");
}



std::pair<float,float> Game::raycast(Vector2D posStart, Vector2D normal, bool findWallFPlayerT) {
    //Start at the start position and move cell by cell on the level in the direction of the input normal.
    //If findWall then find the distance of the closest wall in the specified direction.
    //Otherwise check if the player is found before a wall (basically is the player visible from posStart or not).

    //Create some variables to be used as outputs.
    float distance = 0.0f;
    float fColor = 1.0f;

    //The fColor to be used for a corner.
    const float fColorCorner = 0.3f;

    //Determine how far the edge of the current cell is from posStart in the x direction.
    float deltaX = posStart.x - (int)posStart.x;
    //Ensure that the amount is correct depending on the direction of movement.  Invert it if needed.
    if (normal.x > 0.0f)
        deltaX = 1.0f - deltaX;

    //Do the same for the y direction.
    float deltaY = posStart.y - (int)posStart.y;
    if (normal.y > 0.0f)
        deltaY = 1.0f - deltaY;

    //The current integer position to check in the level.
    int iPosCheckX = (int)posStart.x, iPosCheckY = (int)posStart.y;


    bool firstCheck = true;
    bool checking = true;

    while (checking) {
        //Advance the position to be checked if needed.
        if (firstCheck == true) {
            //If this is the first check then don't advance iPosCheckX or iPosCheckY.
            firstCheck = false;
        }
        else {
            //If we move along the direction normal will the next cell be in the x or y direction?
            float ratioX = 1.0f;
            if (abs(normal.x) > 0.0f)
                ratioX = deltaX / abs(normal.x);

            float ratioY = 1.0f;
            bool ratioYSet = false;
            if (abs(normal.y) > 0.0f) {
                ratioY = deltaY / abs(normal.y);
                ratioYSet = true;
            }

            //Advance in the required direction.
            if (ratioX < ratioY || ratioYSet == false) {
                //The cell in the x direction is closer.
                iPosCheckX += (normal.x > 0 ? 1 : -1);
                deltaX = 1.0f;
                deltaY -= ratioX * abs(normal.y);

                distance += ratioX;

                //Update the color of the wall depending on if it's a corner or not.
                if (deltaY < 0.05f || deltaY > 0.95f)
                    fColor = fColorCorner;
                else
                    fColor = 1.0f;
            }
            else {
                //The cell in the y direction is closer.
                iPosCheckY += (normal.y > 0 ? 1 : -1);
                deltaX -= ratioY * abs(normal.x);
                deltaY = 1.0f;

                distance += ratioY;

                //Update the color of the wall depending on if it's a corner or not.
                if (deltaX < 0.05f || deltaX > 0.95f)
                    fColor = fColorCorner;
                else
                    fColor = 0.5f;
            }
        }


        //Check the specified position iPosCheck.
        int indexCheck = iPosCheckX + iPosCheckY * Level::levelWidth;
        if (indexCheck > -1 && indexCheck < Level::levelSize) {
            //Check if looking for a wall or the player
            if (findWallFPlayerT == false) {
                if (Level::levelData[indexCheck] == Level::symbolWall)
                    //A wall was found so output it's distance and fColor.
                    return std::pair<float, float>(distance, fColor);
                else
                    //No wall was found so ensure that the cell on listVisibleCells is set to true.
                    if (listVisibleCells[indexCheck] == false)
                        listVisibleCells[indexCheck] = true;
            }
            else {
                if (Level::levelData[indexCheck] == Level::symbolWall)
                    //A wall was found before the player so stop checking.
                    checking = false;
                else if (unitPlayer != nullptr && iPosCheckX == (int)unitPlayer->getPos().x && 
                    iPosCheckY == (int)unitPlayer->getPos().y)
                    //No wall was found but the player was so output it's distance and a dummy fColor.
                    return std::pair<float, float>(distance, 1.0f);
            }
        }
        else
            //Out of bounds so stop checking.
            checking = false;
    }

    //Nothing was found so output a negative distance and fColor as 1.0f.
    return std::pair<float, float>(-0.001f, 1.0f);
}



void Game::drawWalls(SDL_Renderer* renderer) {
    if (worldWidth > 0 && unitPlayer != nullptr) {
        //Reset the list of visible cells so that none are visible anymore.
        std::fill(listVisibleCells.begin(), listVisibleCells.end(), false);

        //Loop through the width of the world/screen to draw the walls vertical line by vertical line.
        for (int count = 0; count < worldWidth; count++) {
            const float angleOffset = -fovRad / 2.0f + fovRad * (count / (float)(worldWidth - 1));
            //Determine the angle of the current vertical line to be drawn in the world.
            const float angleCurrent = unitPlayer->getAngle() + angleOffset;

            //Perform a raycast in the direction of the current vertical line to determine how far away it is.
            std::pair<float, float> raycastData = raycast(unitPlayer->getPos(), Vector2D(angleCurrent), false);
            float distance = raycastData.first;
            float fColor = raycastData.second;

            //Ensure that there wasn't an error eg. the player is somehow out of bounds of the world.
            if (distance > 0.0f) {
                //Set the color of the wall to be drawn.
                SDL_SetRenderDrawColor(renderer, 0, 0, (int)round(255 * fColor), 255);

                //Determine the height of the vertical line to be drawn.
                float heightDraw = 1.0f * (atan(0.5f / distance) / fovRad * worldWidth * 2);
                //Set the depth/distance that the current line is away from the player/camera.
                listDepthDraw[count] = distance;

                //The position of the vertical line on the screen to be drawn.
                SDL_Rect rect = {
                    count,
                    (int)((worldHeight - heightDraw) / 2),
                    1,
                    (int)heightDraw };

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}



void Game::addAllSpritesToDrawList(SDL_Renderer* renderer) {
    //Add any of the needed sprites to the draw list.
    for (std::shared_ptr<Sprite> enemy : listUnitEnemies)
        addSpriteToDrawList(enemy);

    for (std::shared_ptr<Sprite> pickup : listPickups)
        addSpriteToDrawList(pickup);

    for (std::shared_ptr<Sprite> projectile : listProjectiles)
        addSpriteToDrawList(projectile);

    addSpriteToDrawList(spriteFlag);
}



void Game::addSpriteToDrawList(std::shared_ptr<Sprite>& sprite) {
    if (unitPlayer != nullptr && sprite != nullptr) {
        //Check if the position of the sprite is visible.
        //Note that this is set by the raycast function when drawWalls is called.
        int indexCheck = (int)(sprite->getPos().x) + (int)(sprite->getPos().y) * Level::levelWidth;
        if (indexCheck > -1 && indexCheck < Level::levelSize &&
            listVisibleCells[indexCheck]) {
            float distance = (unitPlayer->getPos() - sprite->getPos()).magnitude();
            //Add the distance and sprite to the list.
            listSpritesToDraw.push_back(std::pair<float, std::weak_ptr<Sprite>>(
                distance, std::weak_ptr<Sprite>(sprite)));
        }
    }
}



void Game::sortAndDrawListSpritesToDraw(SDL_Renderer* renderer) {
    if (listSpritesToDraw.empty() == false) {
        //Sort the list based on the stored distances.
        std::sort(listSpritesToDraw.begin(), listSpritesToDraw.end(),
            [](auto a, auto b) { return (a.first < b.first); });

        //Loop through the list of sprites and draw them from furthest to closest.
        while (listSpritesToDraw.empty() == false) {
            std::pair<float, std::weak_ptr<Sprite>> dataSelected = listSpritesToDraw.back();
            if (auto spriteSelected = dataSelected.second.lock())
                spriteSelected->draw(renderer, worldWidth, worldHeight, unitPlayer, fovRad, listDepthDraw);

            listSpritesToDraw.pop_back();
        }
    }
}



void Game::drawText(SDL_Renderer* renderer, int offsetX, int offsetY, int size, std::string textToDraw) {
    if (textureFont != nullptr) {
        int cursorOffsetX = 0;
        //Loop through the textToDraw character by character.
        for (auto& charSelected : textToDraw) {
            //Get the ascii keycode for the selected character and find it's position in fontTexture.
            int charIDSelected = int(charSelected);
            int row = charIDSelected / 21;
            int column = charIDSelected % 21;

            //Draw the selected char in the correct spot at the correct size.
            SDL_Rect rSrc = { 1 + 6 * column, 1 + 8 * row, 5, 7 };
            SDL_Rect rDst = { offsetX + cursorOffsetX, offsetY, 5 * size, 7 * size };
            SDL_RenderCopy(renderer, textureFont, &rSrc, &rDst);

            //Advance the cursor.
            cursorOffsetX += 6 * size;
        }
    }
}



void Game::addRandomPickup(SDL_Renderer* renderer, Vector2D pos) {
    //Pick the index of a pickup to add at random.  Note if it's a value above 3 then it won't add anything.
    int index = rand() % (3 + 7);

    switch (index) {
    case 0:
        listPickups.push_back(std::make_shared<Health>(renderer, pos));
        break;
    case 1:
        listPickups.push_back(std::make_shared<Coin>(renderer, pos));
        break;
    case 2:
        listPickups.push_back(std::make_shared<Ammo>(renderer, pos));
        break;
    }
}