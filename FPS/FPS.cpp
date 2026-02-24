#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "olcConsoleGameEngine.h"

class FPSGame : public olcConsoleGameEngine
{
public:
    FPSGame() { m_sAppName = L"3D FPS Game"; }

private:
    struct Enemy
    {
        float x, y;
    };

    struct Bullet
    {
        float x, y, angle;
        float distanceTraveled;
        float maxRange;
        bool active;
        
        Bullet(float startX, float startY, float startAngle, float range)
            : x(startX), y(startY), angle(startAngle), distanceTraveled(0.0f), maxRange(range), active(true) {}
    };

    // Weapon system variables
    int nMagazineSize = 30;           // 弹夹容量
    int nCurrentAmmo = 30;            // 当前弹夹内弹药数
    int nTotalAmmo = 90;              // 总弹药数（不包括弹夹内的）
    float fFireRate = 0.1f;           // 射击间隔（秒）
    float fTimeSinceLastShot = 0.0f;  // 距离上次射击的时间
    float fReloadTime = 2.0f;         // 装弹时间（秒）
    float fReloadTimer = 0.0f;        // 装弹计时器
    bool bIsReloading = false;        // 是否正在装弹
    float fBulletSpeed = 15.0f;       // 子弹速度（独立于玩家移动速度）
    float fBulletMaxRange = 20.0f;    // 子弹最大射程

    int nMapWidth = 16;
    int nMapHeight = 16;
    float fPlayerX = 14.7f;
    float fPlayerY = 5.09f;
    float fPlayerA = 0.0f;
    float fFOV = 3.14159f / 4.0f;
    float fDepth = 16.0f;
    float fSpeed = 5.0f;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::wstring map;

protected:
    virtual bool OnUserCreate()
    {
        map += L"################";
        map += L"#..............#";
        map += L"#.......########";
        map += L"#..............#";
        map += L"#......##......#";
        map += L"#......##......#";
        map += L"#..............#";
        map += L"###............#";
        map += L"##.............#";
        map += L"#......####..###";
        map += L"#......#.......#";
        map += L"#......#.......#";
        map += L"#..............#";
        map += L"#......#########";
        map += L"#..............#";
        map += L"################";

        // Add enemies at fixed positions
        enemies.push_back({ 8.0f, 8.0f });
        enemies.push_back({ 10.0f, 10.0f });

        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime)
    {
        // Handle player movement
        if (GetKey(L'W').bHeld)
        {
            fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;
                fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
            }
        }

        if (GetKey(L'S').bHeld)
        {
            fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;
            fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;
                fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
            }
        }

        if (GetKey(L'A').bHeld)
            fPlayerA -= (fSpeed * 0.75f) * fElapsedTime;

        if (GetKey(L'D').bHeld)
            fPlayerA += (fSpeed * 0.75f) * fElapsedTime;

        // Update reload timer
        if (bIsReloading)
        {
            fReloadTimer += fElapsedTime;
            if (fReloadTimer >= fReloadTime)
            {
                // Reload complete
                int ammoNeeded = nMagazineSize - nCurrentAmmo;
                if (ammoNeeded > 0 && nTotalAmmo > 0)
                {
                    int ammoToReload = std::min(ammoNeeded, nTotalAmmo);
                    nCurrentAmmo += ammoToReload;
                    nTotalAmmo -= ammoToReload;
                }
                bIsReloading = false;
                fReloadTimer = 0.0f;
            }
        }

        // Update fire rate timer
        fTimeSinceLastShot += fElapsedTime;

        // Handle reload input
        if (GetKey(L'R').bPressed && !bIsReloading && nCurrentAmmo < nMagazineSize && nTotalAmmo > 0)
        {
            bIsReloading = true;
            fReloadTimer = 0.0f;
        }

        // Handle shooting (spacebar or left mouse button)
        bool bCanShoot = !bIsReloading && nCurrentAmmo > 0 && fTimeSinceLastShot >= fFireRate;
        if ((GetKey(L' ').bHeld || GetMouse(0).bHeld) && bCanShoot)
        {
            // Fire bullet
            bullets.push_back(Bullet(fPlayerX, fPlayerY, fPlayerA, fBulletMaxRange));
            nCurrentAmmo--;
            fTimeSinceLastShot = 0.0f;
        }

        // Update bullets
        for (auto& b : bullets)
        {
            if (!b.active) continue;

            // Calculate movement distance
            float fMoveDistance = fBulletSpeed * fElapsedTime;
            float fNewX = b.x + cosf(b.angle) * fMoveDistance;
            float fNewY = b.y + sinf(b.angle) * fMoveDistance;
            
            // Update distance traveled
            b.distanceTraveled += fMoveDistance;

            // Check if bullet exceeded max range
            if (b.distanceTraveled >= b.maxRange)
            {
                b.active = false;
                continue;
            }

            // Check for collision with walls using raycast
            bool bHitWall = false;
            float fStepSize = 0.05f;
            float fCheckDistance = 0.0f;
            float fCheckX = b.x;
            float fCheckY = b.y;
            
            while (fCheckDistance < fMoveDistance && !bHitWall)
            {
                fCheckDistance += fStepSize;
                fCheckX = b.x + cosf(b.angle) * fCheckDistance;
                fCheckY = b.y + sinf(b.angle) * fCheckDistance;
                
                int nCheckX = (int)fCheckX;
                int nCheckY = (int)fCheckY;
                
                if (nCheckX < 0 || nCheckX >= nMapWidth || nCheckY < 0 || nCheckY >= nMapHeight)
                {
                    bHitWall = true;
                }
                else if (map[nCheckY * nMapWidth + nCheckX] == '#')
                {
                    bHitWall = true;
                    // Set bullet position to collision point
                    fNewX = fCheckX - cosf(b.angle) * fStepSize;
                    fNewY = fCheckY - sinf(b.angle) * fStepSize;
                }
            }

            if (bHitWall)
            {
                b.active = false;
                continue;
            }

            // Update bullet position
            b.x = fNewX;
            b.y = fNewY;

            // Check for collision with enemies (more precise)
            for (auto& e : enemies)
            {
                if (e.x < 0) continue; // Skip removed enemies
                
                float fDistToEnemy = sqrt((b.x - e.x) * (b.x - e.x) + (b.y - e.y) * (b.y - e.y));
                if (fDistToEnemy < 0.5f)
                {
                    e.x = -1; // Mark enemy for removal
                    b.active = false;
                    break;
                }
            }
        }

        // Remove inactive bullets and dead enemies
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
            [](const Bullet& b) { return !b.active; }), bullets.end());
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), 
            [](const Enemy& e) { return e.x < 0; }), enemies.end());

        // Render the scene
        for (int x = 0; x < ScreenWidth(); x++)
        {
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth()) * fFOV;
            float fStepSize = 0.1f;
            float fDistanceToWall = 0.0f;

            bool bHitWall = false;
            bool bBoundary = false;

            float fEyeX = cosf(fRayAngle);
            float fEyeY = sinf(fRayAngle);

            while (!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall += fStepSize;
                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else
                {
                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;

                        std::vector<std::pair<float, float>> p;

                        for (int tx = 0; tx < 2; tx++)
                            for (int ty = 0; ty < 2; ty++)
                            {
                                float vy = (float)nTestY + ty - fPlayerY;
                                float vx = (float)nTestX + tx - fPlayerX;
                                float d = sqrt(vx * vx + vy * vy);
                                float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
                                p.push_back(std::make_pair(d, dot));
                            }

                        std::sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right)
                            {
                                return left.first < right.first;
                            });

                        float fBound = 0.01f;
                        if (acos(p.at(0).second) < fBound) bBoundary = true;
                        if (acos(p.at(1).second) < fBound) bBoundary = true;
                        if (acos(p.at(2).second) < fBound) bBoundary = true;
                    }
                }
            }

            int nCeiling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceToWall);
            int nFloor = ScreenHeight() - nCeiling;

            short nShade = ' ';
            short nShadeColor = FG_WHITE;

            if (fDistanceToWall <= fDepth / 4.0f) { nShade = 0x2588; nShadeColor = FG_RED; }
            else if (fDistanceToWall < fDepth / 3.0f) { nShade = 0x2593; nShadeColor = FG_RED; }
            else if (fDistanceToWall < fDepth / 2.0f) { nShade = 0x2592; nShadeColor = FG_RED; }
            else if (fDistanceToWall < fDepth) { nShade = 0x2591; nShadeColor = FG_RED; }
            else { nShade = ' '; nShadeColor = FG_WHITE; }

            if (bBoundary) nShade = ' ';

            for (int y = 0; y < ScreenHeight(); y++)
            {
                if (y <= nCeiling)
                    Draw(x, y, ' ');
                else if (y > nCeiling && y <= nFloor)
                    Draw(x, y, nShade, nShadeColor);
                else
                {
                    float b = 1.0f - (((float)y - ScreenHeight() / 2.0f) / ((float)ScreenHeight() / 2.0f));
                    short nFloorShade = ' ';
                    short nFloorColor = FG_WHITE;

                    if (b < 0.25) { nFloorShade = '#'; nFloorColor = FG_GREY; }
                    else if (b < 0.5) { nFloorShade = 'x'; nFloorColor = FG_GREY; }
                    else if (b < 0.75) { nFloorShade = '.'; nFloorColor = FG_GREY; }
                    else if (b < 0.9) { nFloorShade = '-'; nFloorColor = FG_GREY; }
                    else { nFloorShade = ' '; nFloorColor = FG_WHITE; }
                    Draw(x, y, nFloorShade, nFloorColor);
                }
            }
        }

        // Render enemies
        // Render enemies
        for (auto& e : enemies)
        {
            float fVecX = e.x - fPlayerX;
            float fVecY = e.y - fPlayerY;
            float fDistanceFromPlayer = sqrt(fVecX * fVecX + fVecY * fVecY);

            float fEyeX = cosf(fPlayerA);
            float fEyeY = sinf(fPlayerA);

            float fObjectAngle = atan2f(fEyeY, fEyeX) - atan2f(fVecY, fVecX);
            if (fObjectAngle < -3.14159f)
                fObjectAngle += 2.0f * 3.14159f;
            if (fObjectAngle > 3.14159f)
                fObjectAngle -= 2.0f * 3.14159f;

            bool bInFOV = fabs(fObjectAngle) < fFOV / 2.0f;

            if (bInFOV && fDistanceFromPlayer >= 0.5f && fDistanceFromPlayer < fDepth)
            {
                int nObjectCeiling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceFromPlayer);
                int nObjectFloor = ScreenHeight() - nObjectCeiling;
                int nObjectHeight = nObjectFloor - nObjectCeiling;
                int nObjectAspectRatio = 1;
                int nObjectWidth = nObjectHeight / nObjectAspectRatio;

                float fObjectMidX = (0.5f * (fObjectAngle / (fFOV / 2.0f)) + 0.5f) * (float)ScreenWidth();
                for (int lx = 0; lx < nObjectWidth; lx++)
                {
                    for (int ly = 0; ly < nObjectHeight; ly++)
                    {
                        int nObjX = fObjectMidX + lx - (nObjectWidth / 2);
                        int nObjY = nObjectCeiling + ly;

                        // Draw human shape
                        if (lx == nObjectWidth / 2)
                        {
                            Draw(nObjX, nObjY, PIXEL_SOLID, FG_RED); // Body
                        }
                        else if (ly == nObjectHeight / 5)
                        {
                            Draw(nObjX, nObjY, PIXEL_SOLID, FG_RED); // Head
                        }
                        else if (lx == nObjectWidth / 4 || lx == 3 * nObjectWidth / 4)
                        {
                            Draw(nObjX, nObjY, PIXEL_SOLID, FG_RED); // Arms and legs
                        }
                    }
                }
            }
        }

        // Render bullets
        for (auto& b : bullets)
        {
            if (!b.active) continue;
            
            float fVecX = b.x - fPlayerX;
            float fVecY = b.y - fPlayerY;
            float fDistanceFromPlayer = sqrt(fVecX * fVecX + fVecY * fVecY);

            float fEyeX = cosf(fPlayerA);
            float fEyeY = sinf(fPlayerA);

            float fObjectAngle = atan2f(fEyeY, fEyeX) - atan2f(fVecY, fVecX);
            if (fObjectAngle < -3.14159f)
                fObjectAngle += 2.0f * 3.14159f;
            if (fObjectAngle > 3.14159f)
                fObjectAngle -= 2.0f * 3.14159f;

            bool bInFOV = fabs(fObjectAngle) < fFOV / 2.0f;

            if (bInFOV && fDistanceFromPlayer >= 0.5f && fDistanceFromPlayer < fDepth)
            {
                int nObjectCeiling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceFromPlayer);
                int nObjectFloor = ScreenHeight() - nObjectCeiling;
                int nObjectHeight = nObjectFloor - nObjectCeiling;
                int nObjectAspectRatio = 1;
                int nObjectWidth = nObjectHeight / nObjectAspectRatio;

                float fObjectMidX = (0.5f * (fObjectAngle / (fFOV / 2.0f)) + 0.5f) * (float)ScreenWidth();
                for (int lx = 0; lx < nObjectWidth; lx++)
                {
                    for (int ly = 0; ly < nObjectHeight; ly++)
                    {
                        int nObjX = fObjectMidX + lx - (nObjectWidth / 2);
                        int nObjY = nObjectCeiling + ly;
                        Draw(nObjX, nObjY, '*', FG_YELLOW);
                    }
                }
            }
        }

        // Render HUD - Ammo display
        std::wstring sAmmoText = L"Ammo: " + std::to_wstring(nCurrentAmmo) + L" / " + std::to_wstring(nTotalAmmo);
        for (size_t i = 0; i < sAmmoText.length() && i < ScreenWidth(); i++)
        {
            Draw(i, ScreenHeight() - 1, sAmmoText[i], FG_GREEN);
        }

        // Render reload indicator
        if (bIsReloading)
        {
            float fReloadProgress = fReloadTimer / fReloadTime;
            std::wstring sReloadText = L"Reloading... " + std::to_wstring((int)(fReloadProgress * 100)) + L"%";
            int nTextStartX = ScreenWidth() / 2 - sReloadText.length() / 2;
            for (size_t i = 0; i < sReloadText.length() && (nTextStartX + i) < ScreenWidth(); i++)
            {
                Draw(nTextStartX + i, ScreenHeight() / 2, sReloadText[i], FG_YELLOW);
            }
        }

        // Render crosshair
        int nCrosshairX = ScreenWidth() / 2;
        int nCrosshairY = ScreenHeight() / 2;
        Draw(nCrosshairX, nCrosshairY, '+', FG_WHITE);
        Draw(nCrosshairX - 1, nCrosshairY, '-', FG_WHITE);
        Draw(nCrosshairX + 1, nCrosshairY, '-', FG_WHITE);
        Draw(nCrosshairX, nCrosshairY - 1, '|', FG_WHITE);
        Draw(nCrosshairX, nCrosshairY + 1, '|', FG_WHITE);

        return true;
    }
};

int main()
{
    FPSGame game;
    if (game.ConstructConsole(240, 60, 8, 8))
        game.Start();
    return 0;
}