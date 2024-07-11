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
    int nMapWidth = 16;
    int nMapHeight = 16;
    float fPlayerX = 14.7f;
    float fPlayerY = 5.09f;
    float fPlayerA = 0.0f;
    float fFOV = 3.14159f / 4.0f;
    float fDepth = 16.0f;
    float fSpeed = 5.0f;

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
        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime)
    {
        // 四个按键W、A、S、D控制移动
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

        // 渲染
        for (int x = 0; x < ScreenWidth(); x++)
        {
            // 对每一列计算投射到游戏空间的射线角度
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)ScreenWidth()) * fFOV;

            // 射线投射的步长，步长越大，精度越低但渲染速度越快
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

                // 测试射线有没有超出边界
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else
                {
                    // 射线在边界内，测试射线单元是不是为墙壁
                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;

                        // 突出边界，从每个角落投射射线
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

                        // 从最近到最远排序
                        std::sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right)
                            {
                                return left.first < right.first;
                            });

                        // 前两/三个最近（看不见四个）
                        float fBound = 0.01f;
                        if (acos(p.at(0).second) < fBound) bBoundary = true;
                        if (acos(p.at(1).second) < fBound) bBoundary = true;
                        if (acos(p.at(2).second) < fBound) bBoundary = true;
                    }
                }
            }

            // 计算到天花板和地板的距离
            int nCeiling = (float)(ScreenHeight() / 2.0) - ScreenHeight() / ((float)fDistanceToWall);
            int nFloor = ScreenHeight() - nCeiling;

            short nShade = ' ';
            short nShadeColor = FG_WHITE;

            if (fDistanceToWall <= fDepth / 4.0f) { nShade = 0x2588; nShadeColor = FG_RED; }
            else if (fDistanceToWall < fDepth / 3.0f) { nShade = 0x2593; nShadeColor = FG_RED; }
            else if (fDistanceToWall < fDepth / 2.0f) { nShade = 0x2592; nShadeColor = FG_RED; }
            else if (fDistanceToWall < fDepth) { nShade = 0x2591; nShadeColor = FG_RED; }
            else { nShade = ' '; nShadeColor = FG_WHITE; }

            if (bBoundary)       nShade = ' ';

            for (int y = 0; y < ScreenHeight(); y++)
            {
                if (y <= nCeiling)
                    Draw(x, y, ' ');
                else if (y > nCeiling && y <= nFloor)
                    Draw(x, y, nShade, nShadeColor);
                else
                {
                    // 根据距离去渲染地板
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