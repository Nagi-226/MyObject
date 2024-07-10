#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class DOOMClone : public olc::PixelGameEngine
{
public:
    DOOMClone()
    {
        sAppName = "DOOM Clone";
    }

private:
    std::string map;
    int nMapWidth = 16;
    int nMapHeight = 16;

    float fPlayerX = 8.0f;
    float fPlayerY = 8.0f;
    float fPlayerA = 0.0f;

    float fFOV = 3.14159f / 4.0f;
    float fDepth = 16.0f;

public:
    bool OnUserCreate() override
    {
        map += "################";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "#..............#";
        map += "################";
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Handle player movement
        if (GetKey(olc::Key::A).bHeld)
            fPlayerA -= (1.5f) * fElapsedTime;
        if (GetKey(olc::Key::D).bHeld)
            fPlayerA += (1.5f) * fElapsedTime;

        if (GetKey(olc::Key::W).bHeld)
        {
            fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
            }
        }

        if (GetKey(olc::Key::S).bHeld)
        {
            fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
            }
        }

        // Render
        for (int x = 0; x < ScreenWidth(); x++)
        {
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth()) * fFOV;
            float fStepSize = 0.1f;
            float fDistanceToWall = 0.0f;
            bool bHitWall = false;
            bool bBoundary = false;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

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
                    }
                }
            }

            int nCeiling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceToWall);
            int nFloor = ScreenHeight() - nCeiling;

            for (int y = 0; y < ScreenHeight(); y++)
            {
                if (y <= nCeiling)
                    Draw(x, y, olc::BLACK);
                else if (y > nCeiling && y <= nFloor)
                    Draw(x, y, olc::GREY);
                else
                {
                    float b = 1.0f - (((float)y - ScreenHeight() / 2.0f) / ((float)ScreenHeight() / 2.0f));
                    Draw(x, y, olc::PixelF(0.0f, b * 0.5f, 0.0f));
                }
            }
        }

        return true;
    }
};

int main()
{
    DOOMClone demo;
    if (demo.Construct(320, 240, 2, 2))
        demo.Start();
    return 0;
}